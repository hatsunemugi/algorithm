#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "graph.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/algorithm/layout/Main.qml"_qs);
    engine.rootContext()->setContextProperty("graph",new Graph());
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
