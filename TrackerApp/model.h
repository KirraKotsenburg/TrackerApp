#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QImage>
#include <QTimer>
#include <opencv2/opencv.hpp>

class Model : public QObject {
    Q_OBJECT
    Q_PROPERTY(int data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(QImage frame READ frame NOTIFY frameChanged)

public:
    explicit Model(QObject *parent = nullptr); // Constructor with default parameter for parent
    int data() const; // Getter for the data property
    Q_INVOKABLE void setData(int value);  // Setter for the data property
    Q_INVOKABLE void onConnect();
    Q_INVOKABLE void openUART();
    Q_INVOKABLE void writeUART(const QString &input);

    QImage frame() const; // Getter for the frame property

signals:
    void dataChanged(); // Signal to be emitted when data changes
    void frameChanged(); // Signal to be emmited when frame changes

private slots:
    void captureFrame(); // Slot to capture video frames

private:
    int m_data; // Private member to hold the data
    QSerialPort serialPort;

    QTimer *m_timer; // Timer to capture frames at intervals
    QImage m_frame; // Holds the current frame
    cv::VideoCapture m_capture; // OpenCV video capture

    QImage matToQImage(const cv::Mat &mat); // Helper to convert cv::Mat to QImage
};

#endif // MODEL_H
