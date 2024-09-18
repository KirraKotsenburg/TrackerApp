#include <QDebug>
#include "model.h"
#include <QCameraDevice>
#include <QMediaDevices>
#include <QSerialPortInfo>


Model::Model(QObject *parent)
    : QObject(parent)
{
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    numCams = cameras.size();
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    qDebug() << "Number of Ports available: " << ports.size();
    foreach (const QSerialPortInfo &portInfo, ports) {
        availPorts.push_back(portInfo.portName());
    }
}

void Model::accessCamera(int camIndex)
{
    cap.open(camIndex);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    if (!cap.isOpened()) {
        qDebug() << "Cannot open camera.";
    }
    else {
        qDebug() << "Camera opened.";
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

int Model::getNumPorts() {
    return availPorts.size();
}

std::vector<QString> Model::getAvailComPorts() {
    return availPorts;
}

void Model::onConnect() {
    qDebug() << "Connecting...\n";

    timer.start(1000 / 30); // 30 fps
}


void Model::openUART(QString comPort) {
    serialPort.setPortName(comPort);
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
        qDebug() << "Successfully opened UART port on COM.\n";
    }
}


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

void Model::writeUART(const QByteArray &data) {
    serialPort.write(data);
    qDebug() << "Writing data to UART: " << data;
}

void Model::readUART() {
    // Read all available data from the UART buffer
    QByteArray data = serialPort.readAll();
    qDebug() << "Received UART data: " << data.toHex(' ');

    // Ensure that the message starts with the correct header
    if (data.size() < 5 || data[0] != '!') {
        qDebug() << "Invalid message header";
        return;
    }

    char messageID = data[4];
    qDebug() << "Message ID: " << static_cast<int>(messageID);

    // Emit the appropriate signal based on the message ID or payload content
    if (messageID == 'g') {
        emit trackFail();
        qDebug() << "ERROR: Tracking has Failed!!!";
    } else {
        qDebug() << "Received unknown message ID: " << static_cast<int>(messageID);
    }
}


QImage Model::frame() const {
    return m_frame;
}


void Model::startVideo() {
    cv::Mat frame;
    cap >> frame;
    if (!frame.empty()) {
        m_frame = matToQImage(frame);
        emit frameChanged();
        emit imageUpdated();
    } else {
        qDebug() << "No frame captured";
    }
}


QImage Model::matToQImage(const cv::Mat &mat) {
    switch (mat.type()) {
    case CV_8UC4: {
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    case CV_8UC3: {
        // ITS THIS ONE
        QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_8UC1: {
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

