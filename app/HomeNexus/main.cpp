#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Detect embedded mode via CLI flag
    const bool embeddedMode = app.arguments().contains("--embedded");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("embeddedMode", embeddedMode);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("HomeNexus", "Main");

    return app.exec();
}
