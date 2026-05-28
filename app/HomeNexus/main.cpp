#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDir>
#include <QDebug>
#include "AppConfig.hpp"

namespace
{
    QString defaultConfigFilePath()
    {
        const QDir appDir(QCoreApplication::applicationDirPath());
        return appDir.filePath(QStringLiteral("config/homenexus.ini"));
    }
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::setApplicationName(QStringLiteral("HomeNexus"));
    QCoreApplication::setOrganizationName(QStringLiteral("StriebelSoftware"));

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("HomeNexus smart-home dashboard"));
    parser.addHelpOption();

    QCommandLineOption embeddedOption(
        QStringLiteral("embedded"),
        QStringLiteral("Start the application in embedded mode")
        );

    QCommandLineOption configOption(
        QStringList() << QStringLiteral("c") << QStringLiteral("config"),
        QStringLiteral("Path to the HomeNexus .ini config file"),
        QStringLiteral("file")
        );

    parser.addOption(embeddedOption);
    parser.addOption(configOption);
    parser.process(app);

    const bool embeddedMode = parser.isSet(embeddedOption);

    const QString configFilePath =
        parser.isSet(configOption) ? parser.value(configOption) : defaultConfigFilePath();
    qInfo() << "Loading config from:" << configFilePath;

    AppConfig config(configFilePath);

    if (!config.isValid())
    {
        qCritical() << "Invalid application configuration";
    }

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
