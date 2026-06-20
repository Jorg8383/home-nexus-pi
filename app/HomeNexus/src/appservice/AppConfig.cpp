#include "AppConfig.hpp"

#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QSettings>
#include <QCoreApplication>
#include <QString>
#include <QStringLiteral>

#include "AppNotificationTypes.hpp"

AppConfig::AppConfig(const QString &filePath, IAppNotificationClient &notificationClient)
    :m_NotificationClient(notificationClient), m_FilePath(filePath)
{
    loadFromFile(m_FilePath);
    m_IsValid = validate();

    if (!hasApiKey()) {
        qWarning() << "AppConfig::AppConifg -> OpenWeather API key is missing.";
    }
}

void AppConfig::saveWeatherLocation(const QString &city, const QString &country)
{
    QSettings settings(m_FilePath, QSettings::IniFormat);

    settings.beginGroup(QStringLiteral("openweather"));
    settings.setValue(QStringLiteral("city"), city);
    settings.setValue(QStringLiteral("countryCode"), country);
    settings.endGroup();

    settings.sync();

    if (settings.status() != QSettings::NoError)
    {
        qWarning() << "AppConfig::saveWeatherLocation -> error occured while writing: " << m_FilePath;
        return;
    }

    qWarning() << "AppConfig::saveWeatherLocation -> weather location updated."
               << " City:" << city << "Country:" << country;

    m_City = city;
    m_CountryCode = country;
}

bool AppConfig::isValid() const
{
    return configFileExists() && m_IsValid;
}

bool AppConfig::configFileExists() const
{
    return m_ConfigFileExists;
}

bool AppConfig::onlineWeatherEnabled() const
{
    return configFileExists() && hasApiKey();
}

bool AppConfig::hasApiKey() const
{
    return !m_ApiKey.isEmpty();
}

void AppConfig::loadFromFile(const QString &filePath)
{
    const QDir appDir(QCoreApplication::applicationDirPath());
    const QString qWarningPrefix(QStringLiteral("AppConfig::loadFromFile -> "));

    m_WeatherFallbackFilePath = appDir.filePath(QStringLiteral("data/weather/weather_fallback.json"));
    m_ForecastFallbackFilePath = appDir.filePath((QStringLiteral("data/weather/forecast_fallback.json")));

    const QFileInfo configFileInfo(filePath);
    m_ConfigFileExists = configFileInfo.exists();

    if (!m_ConfigFileExists)
    {
        m_NotificationClient.setBannerNotification(AppNotificationTypes::Id::ConfigFileNotFound,
                                                   AppNotificationTypes::Severity::Warning,
                                                   QStringLiteral("Config file 'HomeNexus.ini' not found.")
                                                   );
        qWarning() << qWarningPrefix << "Config file does not exist:" << filePath;
        return;
    }

    QSettings settings(filePath, QSettings::IniFormat);

    m_ApiKey = settings.value(QStringLiteral("openweather/apiKey"), m_ApiKey).toString().trimmed();

    m_Units = settings.value(QStringLiteral("openweather/units"), m_Units).toString().trimmed();

    m_Language
        = settings.value(QStringLiteral("openweather/language"), m_Language).toString().trimmed();

    m_City = settings.value(QStringLiteral("openweather/city"), m_City).toString().trimmed();

    m_CountryCode = settings.value(QStringLiteral("openweather/countryCode"), m_CountryCode)
                        .toString()
                        .trimmed();

    bool ok = false;

    const int requestTimeout = settings
                                   .value(QStringLiteral("openweather/requestTimeoutMs"),
                                          m_RequestTimeoutMs)
                                   .toInt(&ok);

    if (ok)
    {
        m_RequestTimeoutMs = requestTimeout;
    }
    else
    {
        qWarning() << qWarningPrefix
                   << "Invalid integer config value:"
                   << "openweather/requestTimeoutMs"
                   << "- using default:" << m_RequestTimeoutMs;
    }

    ok = false;

    const int currentWeatherInterval = settings
                                           .value(QStringLiteral("openweather/currentWeatherUpdateIntervalMs"),
                                                  m_CurrentWeatherUpdateIntervalMs)
                                           .toInt(&ok);

    if (ok)
    {
        m_CurrentWeatherUpdateIntervalMs = currentWeatherInterval;
    }
    else
    {
        qWarning() << qWarningPrefix
                   << "Invalid integer config value:"
                   << "openweather/currentWeatherUpdateIntervalMs"
                   << "- using default:" << m_CurrentWeatherUpdateIntervalMs;
    }

    ok = false;

    const int forecastInterval = settings
                                     .value(QStringLiteral("openweather/forecastUpdateIntervalMs"),
                                            m_ForecastUpdateIntervalMs)
                                     .toInt(&ok);

    if (ok)
    {
        m_ForecastUpdateIntervalMs = forecastInterval;
    }
    else {
        qWarning() << qWarningPrefix
                   << "Invalid integer config value:"
                   << "openweather/forecastUpdateIntervalMs"
                   << "- using default:" << m_ForecastUpdateIntervalMs;
    }
}

bool AppConfig::validate() const
{
    const QString qWarningPrefix(QStringLiteral("AppConfig::validate -> "));

    bool valid = true;

    if (m_Units.isEmpty())
    {
        qWarning() << qWarningPrefix << "Invalid config value: openweather/units";
        valid = false;
    }

    if (m_Language.isEmpty())
    {
        qWarning() << qWarningPrefix << "Invalid config value: openweather/language";
        valid = false;
    }

    if (m_City.isEmpty())
    {
        qWarning() << qWarningPrefix << "Invalid config value: openweather/city";
        valid = false;
    }

    if (m_CountryCode.isEmpty())
    {
        qWarning() << qWarningPrefix << "Invalid config value: openweather/countryCode";
        valid = false;
    }

    if (m_RequestTimeoutMs <= 0)
    {
        qWarning() << qWarningPrefix << "Invalid config value: openweather/requestTimeoutMs";
        valid = false;
    }

    if (m_CurrentWeatherUpdateIntervalMs <= 0)
    {
        qWarning() << qWarningPrefix
                   << "Invalid config value:"
                   << "openweather/currentWeatherUpdateIntervalMs";
        valid = false;
    }

    if (m_ForecastUpdateIntervalMs <= 0)
    {
        qWarning() << qWarningPrefix
                   << "Invalid config value:"
                   << "openweather/forecastUpdateIntervalMs";
        valid = false;
    }

    return valid;
}

QString AppConfig::apiKey() const
{
    return m_ApiKey;
}

QString AppConfig::units() const
{
    return m_Units;
}

QString AppConfig::language() const
{
    return m_Language;
}

QString AppConfig::city() const
{
    return m_City;
}

QString AppConfig::countryCode() const
{
    return m_CountryCode;
}

QString AppConfig::weatherFallbackFilePath() const
{
    return m_WeatherFallbackFilePath;
}

QString AppConfig::forecastFallbackFilePath() const
{
    return m_ForecastFallbackFilePath;
}

int AppConfig::requestTimeoutMs() const
{
    return m_RequestTimeoutMs;
}

int AppConfig::currentWeatherUpdateIntervalMs() const
{
    return m_CurrentWeatherUpdateIntervalMs;
}

int AppConfig::forecastUpdateIntervalMs() const
{
    return m_ForecastUpdateIntervalMs;
}
