#include <QDebug>
#include "model.h"
#include <QCameraDevice>
#include <QMediaDevices>

Model::Model(QObject *parent)
    : QObject(parent),
    m_data(0) // Initialize m_data with a default value (0 here)
    // cap(0) // What port used determines this number // "USB2.0 PC CAMERA"
{
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    numCams = cameras.size();
    qDebug() << "In model. Number of Cams: " << numCams;
    // cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    // cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    // if (!cap.isOpened()) {
    //     qDebug() << "Cannot open webcam";
    // }
    // else {
    //     qDebug() << "Webcam opened";
    // }

    // connect(&timer, &QTimer::timeout, this, &Model::startVideo);

}

void Model::accessCamera(int camIndex)
{
    cap.open(camIndex);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    if (!cap.isOpened()) {
        qDebug() << "Cannot open webcam";
    }
    else {
        qDebug() << "Webcam opened";
    }

    connect(&timer, &QTimer::timeout, this, &Model::startVideo);
}

void Model::disconnectCamera()
{
    cap.release();
}

int Model::getNumCams(){
    return numCams;
}

int Model::data() const {
    return m_data; // Return the current value of m_data
}

void Model::setData(int value) {
    if (m_data != value) { // Only change m_data if the new value is different
        m_data = value; // Set m_data to the new value
        emit dataChanged(); // Emit the dataChanged signal
    }
}

void Model::onConnect() {
    qDebug() << "Connecting...\n";

    timer.start(1000 / 30); // 30 fps
}

/*
 * Function for setting up UART:
 * Opens Serial Port for UART, sets the Baud, ect.
 * COM6 is Nick's UART, COM5 is Char's
 */
void Model::openUART() {
    serialPort.setPortName("COM5"); // COM6 for Nick's, COM 5 for Char's
    serialPort.setBaudRate(QSerialPort::Baud115200); // Changed 9600 to 115200
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);

    // Connect the readyRead signal to a slot
    connect(&serialPort, &QSerialPort::readyRead, this, &Model::readUART);

    // Open serialPort
    if (!serialPort.open(QIODevice::ReadWrite)) {
        qDebug() << "Error opening serial port.\n";
    }
    else {
        qDebug() << "Successfully opened UART port on COM5.\n";
    }
}

/*
* Prepares the payload for sending, by filling a buffer with the required header
* A pre-assigned region of memory of size (payload_bytes + 5) must be reserved. The first byte of this buffer is passed to char* buffer.
* The payload is a series of arbitrary bytes, total bytes in payload must be less than 62.
* messageID should match the purpose of the message, as defined by external documentation
*/
void Model::payloadPrepare(const QString& payload, char messageID) {
    // Calculate the number of characters in the payload (UTF-8 encoded size)
    QByteArray encodedPayload = payload.toUtf8();
    uint16_t payloadSize = static_cast<uint16_t>(encodedPayload.size());

    // Calculate the 1's complement of the payload
    int sum = 0;
    for (char c : encodedPayload) {
        sum += static_cast<uint8_t>(c);
    }
    uint8_t onesComplement = static_cast<uint8_t>(0xFF - sum);

    // Allocate the buffer dynamically using QByteArray
    QByteArray buffer;
    buffer.resize(5 + payloadSize);

    // Fill the buffer with header information
    buffer[0] = '!';
    buffer[1] = static_cast<char>(payloadSize & 0xFF); // Lower 8 bits of payloadSize
    buffer[2] = static_cast<char>((payloadSize >> 8) & 0xFF); // High 8 bits of payloadSize
    buffer[3] = static_cast<char>(onesComplement);
    buffer[4] = messageID;

    // Copy the payload into the buffer starting from index 5
    memcpy(buffer.data() + 5, encodedPayload.data(), encodedPayload.size());
    qDebug() << "Unpadded Buffer: " << buffer.toHex(' ');

    // Add padding to reach a total of 32 bytes
    int paddingSize = 32 - buffer.size()%32;
    if (paddingSize > 0) {
        buffer.append(QByteArray(paddingSize, 0x00)); // Append padding with 0x00 (null bytes)
    }

    // Debugging outputs (Can remove/comment this out to make terminal output less busy)
    qDebug() << "Buffer 1 (Payload Size Low Byte): " << static_cast<int>(buffer[1]);
    qDebug() << "Buffer 2 (Payload Size High Byte): " << static_cast<int>(buffer[2]);
    qDebug() << "Buffer 3 (One's Complement): " << static_cast<int>(buffer[3]);
    qDebug() << "Buffer 4 (Message ID): " << static_cast<int>(buffer[4]);
    qDebug() << "Payload Size: " << payloadSize;
    qDebug() << "Encoded Payload: " << encodedPayload;
    qDebug() << "Full Buffer: " << buffer.toHex(' '); // Print buffer as hex for better visibility

    // Send the buffer over UART
    writeUART(buffer);
}

/*
* Writes our buffer containing the command message over
* UART to the raspi
*/
    void Model::writeUART(const QByteArray &data) {
        serialPort.write(data);
        qDebug() << "Writing data to UART: " << data;
    }

/*
* Reads UART message sent from the Raspi
* Should only be receiving Track-Fail message
*/
    void Model::readUART() {
        // Read all available data from the UART buffer
        QByteArray data = serialPort.readAll();
        qDebug() << "Received UART data: " << data.toHex(' ');

        // Ensure that the message starts with the correct header
        if (data.size() < 5 || data[0] != '!') {
            qDebug() << "Invalid message header";
            return;
        }

        // // Extract the payload size from the message
        // uint16_t payloadSize = static_cast<uint8_t>(data[1]) | (static_cast<uint8_t>(data[2]) << 8);
        // qDebug() << "Extracted payload size: " << payloadSize;

        // // Ensure the data is long enough to include the full payload
        // if (data.size() < (5 + payloadSize)) {
        //     qDebug() << "Incomplete message received";
        //     return;
        // }

        // Extract the one's complement checksum
        // uint8_t onesComplement = static_cast<uint8_t>(data[3]);

        // Extract the message ID
        char messageID = data[4];
        qDebug() << "Message ID: " << static_cast<int>(messageID);

        // Extract the payload
        // QByteArray payload = data.mid(5, payloadSize);
        // qDebug() << "Extracted payload: " << payload;

        // Verify checksum
        // int sum = 0;
        // for (char c : payload) {
        //     sum += static_cast<uint8_t>(c);
        // }
        // uint8_t calculatedChecksum = static_cast<uint8_t>(0xFF - sum);
        // if (calculatedChecksum != onesComplement) {
        //     qDebug() << "Checksum mismatch";
        //     return;
        // }

        // Emit the appropriate signal based on the message ID or payload content
        if (messageID == 'g') {
            emit trackFail();
            qDebug() << "ERROR: Tracking has Failed!!!";
        } else {
            qDebug() << "Received unknown message ID: " << static_cast<int>(messageID);
        }
    }

QImage Model::frame() const {
    //qDebug() << "returning frame from model...";
    return m_frame;
}

void Model::startVideo() {
    cv::Mat frame;
    cap >> frame;
    if (!frame.empty()) {
        m_frame = matToQImage(frame);
        emit frameChanged();
        //qDebug() << "Frame captured and signal emitted";
        emit imageUpdated();
    } else {
        qDebug() << "No frame captured";
    }
}

QImage Model::matToQImage(const cv::Mat &mat) {
    // Convert cv::Mat to QImage
    switch (mat.type()) {
    case CV_8UC4: {
        // qDebug() << "mat type is CV_8UC4 " << CV_8UC4;
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    case CV_8UC3: {
        // ITS THIS ONE
        // qDebug() << "mat type is CV_8UC3 " << CV_8UC3;
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        // image.rgbSwapped().save("C:/Users/charl/OneDrive/Desktop/TrackerApp/TrackerApp");
        return image.rgbSwapped();
    }
    case CV_8UC1: {
        // qDebug() << "mat type is CV_8UC1 " << CV_8UC1;
        static QVector<QRgb>  colorTable;
        if (colorTable.isEmpty()) {
            for (int i = 0; i < 256; ++i)
                colorTable.push_back(qRgb(i, i, i));
        }
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        image.setColorTable(colorTable);
        return image.copy();
    }
    default:
        break;
    }
    return QImage();
}

void Model::saveFrame(const QString &fileName) {
    if (!m_frame.isNull()) {
        if (m_frame.save(fileName)) {
            qDebug() << "Frame saved to" << fileName;
        } else {
            qDebug() << "Failed to save frame to" << fileName;
        }
    } else {
        qDebug() << "No frame to save";
    }
}

