#include "imageprovider.h"
#include <QDebug>

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image) {}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    Q_UNUSED(id);
    if (size)
        *size = m_image.size();
    return m_image;
}

void ImageProvider::setImage(const QImage &image) {
    m_image = image;
}
