#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDir>
#include <QDebug>
#include <QNetworkAccessManager>

#include "AppConfig.hpp"
#include "OpenWeatherClient.hpp"
#include "GeoCodingClient.hpp"
#include "WeatherService.hpp"
#include "WeatherRepository.hpp"
#include "WeatherViewModel.hpp"
#include "WeatherFallbackProvider.hpp"

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

    const QString configFilePath =
        parser.isSet(configOption) ? parser.value(configOption) : defaultConfigFilePath();
    qInfo() << "Loading config from:" << configFilePath;

    AppConfig config(configFilePath);

    if (!config.isValid())
    {
        qCritical() << "Invalid application configuration";
    }

    QNetworkAccessManager networkManager;

    OpenWeatherClient weatherClient(networkManager, config);
    GeoCodingClient geoCodingClient(networkManager, config);

    WeatherService weatherService(geoCodingClient, weatherClient);
    WeatherFallbackProvider weatherFallback;
    WeatherRepository weatherRepository(weatherService, weatherFallback, config);
    WeatherViewModel weatherViewModel(weatherRepository);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("embeddedMode", embeddedMode);
    engine.rootContext()->setContextProperty("weatherViewModel", &weatherViewModel);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("HomeNexus", "Main");

    // TODO: this is just temporary and will later be triggered by user interaction
    weatherViewModel.updateWeatherForCity(
        config.city(),
        config.countryCode()
        );

    return app.exec();
}
