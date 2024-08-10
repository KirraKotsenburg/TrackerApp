#include <QDebug>
#include "model.h"

Model::Model(QObject *parent)
    : QObject(parent),
    m_data(0), // Initialize m_data with a default value (0 here)
    cap(0) // What port used determines this number // "USB2.0 PC CAMERA"
{
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

void Model::openUART() {
    serialPort.setPortName("COM5");
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
Prepares the payload for sending, by filling a buffer with the required header
A pre-assigned region of memory of size (payload_bytes + 5) must be reserved. The first byte of this buffer is passed to char* buffer.
The payload is a series of arbitrary bytes, total bytes in payload must be less than 62.
messageID should match the purpose of the message, as defined by external documentation

*/
void Model::payloadPrepare(const QString& payload, char messageID) {
    uint16_t payloadSize = static_cast<uint16_t>(payload.size());

    int sum = 0;
    for (QChar c : payload) {
        sum += static_cast<uint8_t>(c.toLatin1());
    }
    uint8_t twosComplement = static_cast<uint8_t>(~sum + 1);

    QByteArray buffer;
    buffer.resize(5 + payloadSize);

    buffer[0] = '!';
    buffer[1] = static_cast<char>(payloadSize & 0xFF);
    buffer[2] = static_cast<char>((payloadSize >> 8) & 0xFF);
    buffer[3] = static_cast<char>(twosComplement);
    buffer[4] = messageID;

    // Properly encoded payload
    QByteArray encodedPayload = payload.toUtf8();
    memcpy(buffer.data() + 5, encodedPayload.data(), encodedPayload.size());

    writeUART(buffer);
}


// void Model::writeUART(const QString &input) {
//     //track-start 448 261 528 381\n
//     QByteArray byteArray = input.toUtf8();
//     qDebug() << "byteArray: " << byteArray;
//     serialPort.write(byteArray);
// }
    void Model::writeUART(const QByteArray &data) {
        qDebug() << "Writing data to UART: " << data;
        serialPort.write(data);
    }


void Model::readUART() {
    while (serialPort.canReadLine()) {
        QByteArray line = serialPort.readLine();
        qDebug() << "Received UART data: " << line;
        //Some sort of logic for a tracking failed signal from raspi

        if(line == "D track-fail\n"){
            emit trackFail();
            qDebug() << "ERROR: Tracking has Failed!!!";
        }

        // TODO: Process the received data as needed
        // Handle error code sent from raspi


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

