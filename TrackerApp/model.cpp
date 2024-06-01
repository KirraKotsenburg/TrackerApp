#include <QDebug>
#include "model.h"

Model::Model(QObject *parent)
    : QObject(parent),
    m_data(0), // Initialize m_data with a default value (0 here)
    m_timer(new QTimer(this)),
    m_capture(0)
{
    connect(m_timer, &QTimer::timeout, this, &Model::captureFrame);
    m_timer->start(30); // Capture frame every 30 ms
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

void Model::captureFrame(){
    cv::Mat frame;
    m_capture >> frame; // Capture a frame
    if(!frame.empty()){
        qDebug() << "Frame captured: " << frame.cols << "x" << frame.rows;
        m_frame = matToQImage(frame);
        emit frameChanged(); // Emit the frameChanged signal
        qDebug() << "frameChanged signal emitted";
    }
}

QImage Model::matToQImage(const cv::Mat &mat){
    // Convert cv::Mat to QImage
    QImage img;
    if (mat.type() == CV_8UC3) {
        img = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888).rgbSwapped();
    } else if (mat.type() == CV_8UC1) {
        img = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
    }
    qDebug() << "Converted frame to QImage: " << img.size();
    return img;
}


