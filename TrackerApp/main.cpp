#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "model.h"
#include "imageprovider.h"
#include <QCameraDevice>
#include <QMediaDevices>

int getCameraIndex (std::string cameraName){
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    int index = -1;
    for (const QCameraDevice &cameraDevice : cameras) {
        qDebug() << "Camera Device Description: " << cameraDevice.description();
        if (cameraDevice.description() == "USB2.0 PC CAMERA"){
            return index;
        }
        index++;
     }

    return -2;
}

int main(int argc, char *argv[]) {
    // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    std::string cameraName = "USB2.0 PC CAMERA";

    int cameraIndex = getCameraIndex(cameraName);
    if (cameraIndex == -2){
        qDebug() << "Camera not found!";
    }

    Model myModel(cameraIndex); // Create an instance of Model
    // Expose the Model instance to QML
    engine.rootContext()->setContextProperty("myModel", &myModel);

    ImageProvider *imageProvider = new ImageProvider();
    // Register the image provider with the engine
    engine.addImageProvider("imageProvider", imageProvider);

    // Connect frameChanged signal to update image in the provider
    QObject::connect(&myModel, &Model::frameChanged, [&myModel, imageProvider]() {
        imageProvider->setImage(myModel.frame());
        //qDebug() << "Frame changed and image provider updated";
    });

    // Connect imageUpdated signal to a QML slot
    QObject::connect(&myModel, &Model::imageUpdated, &engine, [&engine]() {
        QObject *rootObject = engine.rootObjects().first();
        QMetaObject::invokeMethod(rootObject, "updateImageSource");
    });

    // Load the main QML file
    const QUrl url(QStringLiteral("qrc:/TrackerApp/Main.qml")); // Make sure the path matches your QML file location
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
