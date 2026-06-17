#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDir>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QDateTime>

#include "AppConfig.hpp"
#include "AppNotificationClient.hpp"
#include "OpenWeatherClient.hpp"
#include "GeoCodingClient.hpp"
#include "WeatherService.hpp"
#include "WeatherRepository.hpp"
#include "WeatherViewModel.hpp"
#include "WeatherFallbackProvider.hpp"
#include "AppNotificationTypes.hpp"
#include "AppNotificationCenter.hpp"
#include "IAppNotificationClient.hpp"

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
    QCoreApplication::setApplicationVersion("0.1");

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

    qDebug() << "CurrentDateTimeUtc: " << QDateTime::currentDateTimeUtc().toString();
    qDebug() << "CurrentDateTimeLocal: " << QDateTime::currentDateTime().toString();

    const QString configFilePath =
        parser.isSet(configOption) ? parser.value(configOption) : defaultConfigFilePath();
    qInfo() << "Loading config from:" << configFilePath;

    AppConfig config(configFilePath);

    if (!config.isValid())
    {
        qWarning() << "Invalid application configuration";
    }

    QNetworkAccessManager networkManager;

    AppNotificationCenter appNotificationCenter;
    AppNotificationClient appNotificationClient(appNotificationCenter);


    OpenWeatherClient weatherClient(networkManager, config);
    GeoCodingClient geoCodingClient(networkManager, config);

    WeatherService weatherService(geoCodingClient, weatherClient);
    WeatherFallbackProvider weatherFallback(config);
    WeatherRepository weatherRepository(weatherService, weatherFallback, config, appNotificationClient);
    WeatherViewModel weatherViewModel(weatherRepository);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("embeddedMode", embeddedMode);
    engine.rootContext()->setContextProperty("weatherViewModel", &weatherViewModel);
    engine.rootContext()->setContextProperty("appNotificationCenter", &appNotificationCenter);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("HomeNexus", "Main");

    weatherViewModel.updateWeatherForCity(
        config.city(),
        config.countryCode()
        );

    return app.exec();
}
