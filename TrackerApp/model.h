#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

class Model : public QObject {
    Q_OBJECT
    Q_PROPERTY(int data READ data WRITE setData NOTIFY dataChanged)

public:
    explicit Model(QObject *parent = nullptr); // Constructor with default parameter for parent
    int data() const; // Getter for the data property
    Q_INVOKABLE void setData(int value);  // Setter for the data property
    Q_INVOKABLE void onConnect();
    Q_INVOKABLE void openUART();
    Q_INVOKABLE void writeUART();

signals:
    void dataChanged(); // Signal to be emitted when data changes

private:
    int m_data; // Private member to hold the data
    QSerialPort serialPort;
};

#endif // MODEL_H
