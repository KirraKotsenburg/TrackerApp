#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>

/**
 * @authors Kirra Kotsenburg, Charlotte Saethre
 * @date 10/1/2024
 * @brief Class to render images in QML dynamically.
 *
 * This class is responsible for providing images (specifically QImage objects)
 * from the backend to QML. It is particularly useful for displaying
 * dynamically changing images, such as a live video stream, in the QML user interface.
 *
 * In this project, ImageProvider is used to provide video frames captured
 * from a camera (using OpenCV) to the QML interface for real-time display.
 */
class ImageProvider : public QQuickImageProvider {
public:
    /**
     * @brief Constructor for ImageProvider.
     *
     * Initializes the ImageProvider to handle QImage types.
     */
    ImageProvider();
    /**
     * @brief Called whenever QML needs to display image
     *
     * This function is called by QML when an image source like "image://imageProvider/frame"
     * is used in QML. It returns the current video frame stored in the m_image variable.
     *
     * @param id Identifier for image request (not used in this implementation)
     * @param size Pointer to store the size of the image
     * @param requestedSize Size of image requested by QML
     * @return QImage stored in property m_image, current image to be displayed
     */
    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    /**
     * @brief Sets the QImage stored in property m_image
     *
     * This method is used to update the image (e.g., a new video frame from the camera)
     * which will then be provided to QML when requested.
     *
     * @param image The new QImage to be displayed
     */
    void setImage(const QImage &image);

private:
    QImage m_image;
};

#endif // IMAGEPROVIDER_H
