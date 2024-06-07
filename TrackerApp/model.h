#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QImage>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <QLabel>

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
    Q_INVOKABLE void saveFrame(const QString &fileName); // Method to save the frame
    QImage frame() const;

public slots:
    void startVideo();

signals:
    void dataChanged(); // Signal to be emitted when data changes
    void frameChanged();
    void imageUpdated();

private:
    int m_data; // Private member to hold the data
    QSerialPort serialPort;

    QImage matToQImage(const cv::Mat &mat);

    cv::VideoCapture cap;
    QTimer timer;
    QImage m_frame;
};

#endif // MODEL_H
