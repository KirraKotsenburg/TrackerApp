#include <QDebug>
#include "model.h"

Model::Model(QObject *parent)
    : QObject(parent),
    m_data(0) // Initialize m_data with a default value (0 here)
{
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
