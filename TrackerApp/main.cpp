#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "model.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Model myModel; // Create an instance of Model
    engine.rootContext()->setContextProperty("myModel", &myModel); // Expose it to QML

    const QUrl url(QStringLiteral("qrc:/TrackerApp/Main.qml")); // Make sure the path matches your QML file location
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
