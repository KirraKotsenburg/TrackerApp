#include <QDebug>
#include "model.h"

Model::Model(QObject *parent)
    : QObject(parent),
    m_data(0), // Initialize m_data with a default value (0 here)
    cap(0)
{
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    if (!cap.isOpened()) {
        qDebug() << "Cannot open webcam";
    }

    connect(&timer, &QTimer::timeout, this, &Model::startVideo);
    timer.start(1000 / 30); // 30 fps
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
}

void Model::openUART() {
    serialPort.setPortName("COM3");
    serialPort.setBaudRate(QSerialPort::Baud9600);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    // Open serialPort
    if (!serialPort.open(QIODevice::ReadWrite)) {
        qDebug() << "Error opening serial port.\n";
    }
    else {
        qDebug() << "Successfully opened UART port on COM3.\n";
    }
}

void Model::writeUART(const QString &input) {
    //track-start 448 261 528 381\n
    QByteArray byteArray = input.toUtf8();
    qDebug() << "byteArray: " << byteArray;
    serialPort.write(byteArray);
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
    }
}

QImage Model::matToQImage(const cv::Mat &mat) {
    return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_BGR888).copy();
}

