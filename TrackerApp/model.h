#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QImage>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <QLabel>
#include <vector>

/**
 * @authors Kirra Kotsenburg, Charlotte Saethre
 * @date 10/1/2024
 *
 *
 * @brief Class to create Model object
 *
 * The Model class is in charge of the initialization
 * of UART and video capture for the app, as well as ccreating
 * the messages sent to the raspberry pi.
 *
 *
 * Functions:
 *  - explicit Model(QObject *parent = nullptr);
 *  - Q_INVOKABLE void onConnect();
 *  - Q_INVOKABLE bool portOpen();
 *  - Q_INVOKABLE bool getCamOpen();
 *  - Q_INVOKABLE int openUART(QString comPort);
 *  - Q_INVOKABLE void payloadPrepare(const QString& payload, char messageID);
 *  - Q_INVOKABLE void writeUART(const QByteArray &data);
 *  - Q_INVOKABLE void readUART();
 *  - QImage frame() const;
 *  - Q_INVOKABLE int getNumCams();
 *  - Q_INVOKABLE int getNumPorts();
 *  - Q_INVOKABLE std::vector<QString> getAvailComPorts();
 *  - Q_INVOKABLE void accessCamera(int camIndex);
 *  - Q_INVOKABLE void disconnectCamera();
 *  - void startVideo();
 *  - QImage matToQImage(const cv::Mat &mat);
 *
 *
 * Signals:
 *  - void frameChanged();
 *  - void imageUpdated();
 *  - void trackFail();
 *
 *
 * Variables:
 *  - int numCams;
 *  - bool camOpen = false;
 *  - QSerialPort serialPort;
 *  - cv::VideoCapture cap;
 *  - QTimer timer;
 *  - QImage m_frame;
 *  - std::vector<QString> availPorts
 *
 */
class Model : public QObject {
    Q_OBJECT
    Q_PROPERTY(QImage frame READ frame NOTIFY frameChanged)

public:

    /**
     * @brief Constructor for the Model class
     *
     * Gets the number of cameras and COM ports
     * available to the user for selection in the app.
     *
     * @param default parameter for parent
     */
    explicit Model(QObject *parent = nullptr);

    /**
     * Starts the timer to return frame at rate of 30 FPS.
    */
    Q_INVOKABLE void onConnect();

    /**
     * Gets whether the port was opened.
     * @return True if port opened successfully, False otherwise.
     */
    Q_INVOKABLE bool portOpen();

    /**
     * Gets whether the camera opened.
     * @return True if camera opened successfully, False otherwise.
     */
    Q_INVOKABLE bool getCamOpen();

    /**
     * Function for setting up UART:
     * Opens Serial Port for UART, sets the Baud, data bits,
     * parity bits, and any stop bits.
     *
     * @param Qstring input of the COM port being opened.
     * @return An int of either 1 for an error or 0 for success
     * opening the port.
     */
    Q_INVOKABLE int openUART(QString comPort);

    /**
    *  @brief Prepares the payload message intended for Raspi/FC
    *
    * Prepares the payload for sending by filling a buffer with the required header.
    * A pre-assigned region of memory of size (payload_bytes + 5) must be reserved. The first byte of this buffer is passed to char* buffer.
    * The payload is a series of arbitrary bytes, total bytes in payload must be less than 62.
    * messageID should match the purpose of the message, as defined by external documentation.
    *
    * @param Qstring of payload message to be sent.
    * @param MessageID, as defined in external documentation, is either 'e' for track-start
    * or 'f' for track-stop.
    */
    Q_INVOKABLE void payloadPrepare(const QString& payload, char messageID);

    /**
    * Writes the buffer containing the command message over
    * UART to the raspi.
    *
    * @param A QByteArray of the data being sent over UART.
    */
    Q_INVOKABLE void writeUART(const QByteArray &data);

    /**
    * Reads UART message sent from the Raspi.
    * Should only be receiving Track-Fail message.
    */
    Q_INVOKABLE void readUART();

    /**
     * Returns the frame. Function is used to set signals for further operation.
    */
    QImage frame() const;

    /**
     * @brief Variable for number of cameras.
     *
     * Variable holds the number of cameras a user has
     * available for use.
     */
    int numCams;

    /**
     * @brief Boolean for whether Camera opened.
     *
     * Will be set to true if the camera successfully opened.
     * False otherwise.
     */
    bool camOpen = false;

    /**
    * Returns the number of cameras on user's device.
    *
    * @return Number of cameras.
    */
    Q_INVOKABLE int getNumCams();

    /**
     * Gets the number of ports user has.
     *
     * @return The number of ports available.
     */
    Q_INVOKABLE int getNumPorts();

    /**
     * Gets the available COM ports for the use of
     * UART.
     *
     * @return All the COM ports that are available in a Qstring list.
     */
    Q_INVOKABLE std::vector<QString> getAvailComPorts();

    /**
     * Opens camera that user selects via drop down and sets the
     * specifications for the frame width and height.
     * Prints whether camera was opened successfully.
     *
     * @param The index for the camera the user selects.
    */
    Q_INVOKABLE void accessCamera(int camIndex);

    /**
    * Releases the video capture.
    */
    Q_INVOKABLE void disconnectCamera();

public slots:
    /**
     * Starts the video capture. If frame is not empty then updates the emitted signals
     * necessary for continued video streaming.
    */
    void startVideo();

signals:
    /**
     * Signals that the frame changed.
     */
    void frameChanged();

    /**
     * Signals that image was updated.
     */
    void imageUpdated();

    /**
     * @brief Signals that tracking has failed.
     *
     * The signal will be emitted when ReadUART()
     * receives a track-fail message from the raspberry pi.
     * The signal is used in Main.qml as an indicator to
     * reset the app and alert the user to the situation.
     *
     * @see readUART()
     * @see Main.qml
     */
    void trackFail();

private:

    /**
     * QSerialPort variable for the port to be opened.
     */
    QSerialPort serialPort;

/**
 * Performs conversion of OpenCV's mat type into QImage for
 * in app display of video captured.
 *
 * @param The mat type to be converted.
 * @return QImage to be displayed.
*/
    QImage matToQImage(const cv::Mat &mat);

    /**
     * Video Capture object.
     */
    cv::VideoCapture cap;

    /**
     * Timer object used in onConnect().
     *
     * @see onConnect()
     */
    QTimer timer;

    /**
     * Variable stores the converted mat to QImage returned conversion.
     * It is the QImage object returned in frame() for setting other signals.
     *
     * @see frame()
     * @see matToQImage()
     */
    QImage m_frame;

    /**
     * A list (vector) container of QStrings for storing the names
     * of the available COM ports.
     *
     * @see getAvailComPorts()
     */
    std::vector<QString> availPorts;
};

#endif // MODEL_H
