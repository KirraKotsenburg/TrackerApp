#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QImage>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <QLabel>
#include <vector>

class Model : public QObject {
    Q_OBJECT
    Q_PROPERTY(QImage frame READ frame NOTIFY frameChanged)

public:
    explicit Model(QObject *parent = nullptr); // Constructor with default parameter for parent

/*
 * Starts the timer to return frame at rate of 30 FPS.
*/
    Q_INVOKABLE void onConnect();
    Q_INVOKABLE bool portOpen();
    Q_INVOKABLE bool getCamOpen();

/*
 * Function for setting up UART:
 * Opens Serial Port for UART, sets the Baud, data bits,
 * parity bits, and any stop bits.
 */
    Q_INVOKABLE int openUART(QString comPort);

/*
* Prepares the payload for sending, by filling a buffer with the required header
* A pre-assigned region of memory of size (payload_bytes + 5) must be reserved. The first byte of this buffer is passed to char* buffer.
* The payload is a series of arbitrary bytes, total bytes in payload must be less than 62.
* messageID should match the purpose of the message, as defined by external documentation
*/
    Q_INVOKABLE void payloadPrepare(const QString& payload, char messageID);

/*
* Writes our buffer containing the command message over
* UART to the raspi.
*/
    Q_INVOKABLE void writeUART(const QByteArray &data);

/*
* Reads UART message sent from the Raspi
* Should only be receiving Track-Fail message
*/
    Q_INVOKABLE void readUART();

/*
 * Returns the frame. Function is used to set signals for further operation.
*/
    QImage frame() const;
    int numCams;
    bool camOpen = false;

/*
* Returns the number of cameras on user's device.
*/
    Q_INVOKABLE int getNumCams();
    Q_INVOKABLE int getNumPorts();
    Q_INVOKABLE std::vector<QString> getAvailComPorts();

/*
 * Opens camera that user selects via drop down and sets the
 * specifications for the frame width and height.
 * Prints whether camera was opened successfully.
 *
*/
    Q_INVOKABLE void accessCamera(int camIndex);

/*
* Releases the video capture.
*/
    Q_INVOKABLE void disconnectCamera();

public slots:
/*
 * Starts the video capture. If frame is not empty then updates the emitted signals
 * necessary for continued video streaming.
*/
    void startVideo();

signals:
    void frameChanged();
    void imageUpdated();
    void trackFail();

private:
    QSerialPort serialPort;

/*
 * Performs conversion of OpenCV's mat type into QImage for
 * in app display of video captured.
*/
    QImage matToQImage(const cv::Mat &mat);

    cv::VideoCapture cap;
    QTimer timer;
    QImage m_frame;
    std::vector<QString> availPorts;
};

#endif // MODEL_H
