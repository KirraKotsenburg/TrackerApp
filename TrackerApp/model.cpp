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
    else {
        qDebug() << "Webcam opened";
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
    qDebug() << "returning frame from model...";
    return m_frame;
}

void Model::startVideo() {
    cv::Mat frame;
    cap >> frame;
    if (!frame.empty()) {
        m_frame = matToQImage(frame);
        emit frameChanged();
        qDebug() << "Frame captured and signal emitted";
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

