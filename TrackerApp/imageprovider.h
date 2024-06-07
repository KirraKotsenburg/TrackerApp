#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>

class ImageProvider : public QQuickImageProvider {
public:
    ImageProvider();
    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    void setImage(const QImage &image);

private:
    QImage m_image;
};

#endif // IMAGEPROVIDER_H
