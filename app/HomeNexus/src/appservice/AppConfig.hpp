#pragma once

#include <QSettings>
#include <QString>
#include <QUrl>

#include "IAppConfig.hpp"
#include "IAppNotificationClient.hpp"

class AppConfig final : public IAppConfig
{
public:
    explicit AppConfig(const QString& filePath, IAppNotificationClient &notificationClient);

    AppConfig(const AppConfig &) = delete;
    AppConfig &operator=(const AppConfig &) = delete;

    bool isValid() const;

    // IOpenWeatherConfig
    bool configFileExists() const override;
    bool onlineWeatherEnabled() const override;
    bool hasApiKey() const override;
    QString apiKey() const override;
    QString units() const override;
    QString language() const override;
    QString city() const override;
    QString countryCode() const override;
    QString weatherFallbackFilePath() const override;
    QString forecastFallbackFilePath() const override;
    int requestTimeoutMs() const override;
    int currentWeatherUpdateIntervalMs() const override;
    int forecastUpdateIntervalMs() const override;

private:
    void loadFromFile(const QString &filePath);
    bool validate() const;

    bool m_IsValid = false;
    bool m_ConfigFileExists = false;

    QString m_ApiKey = QString();
    QString m_Units = QStringLiteral("metric");
    QString m_Language = QStringLiteral("en");
    QString m_City = QStringLiteral("Mehrstetten");
    QString m_CountryCode = QStringLiteral("DE");
    QString m_WeatherFallbackFilePath;
    QString m_ForecastFallbackFilePath;

    int m_RequestTimeoutMs = 5000;
    int m_CurrentWeatherUpdateIntervalMs = 600000;
    int m_ForecastUpdateIntervalMs = 1800000;

    IAppNotificationClient &m_NotificationClient;
};
