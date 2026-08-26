#pragma once

#include <QSettings>
#include <QString>
#include <QUrl>

#include "IAppConfig.hpp"
#include "IAppNotificationClient.hpp"

class AppConfig final : public IAppConfig
{
public:
    explicit AppConfig(const QString &filePath, IAppNotificationClient &notificationClient);
    ~AppConfig() override = default;

    AppConfig(const AppConfig &) = delete;
    AppConfig &operator=(const AppConfig &) = delete;
    AppConfig(AppConfig &&) = delete;
    AppConfig &operator=(AppConfig &&) = delete;

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
    int weatherUpdateIntervalMs() const override;
    void saveWeatherLocation(const QString &city, const QString &country) override;

private:
    void loadFromFile(const QString &filePath);
    bool validate() const;

    QString m_FilePath;

    bool m_IsValid = false;
    bool m_ConfigFileExists = false;

    QString m_ApiKey = QString();
    QString m_Units = QStringLiteral("metric");
    QString m_Language = QStringLiteral("en");
    QString m_City = QStringLiteral("Mehrstetten");
    QString m_CountryCode = QStringLiteral("DE");
    QString m_WeatherFallbackFilePath;
    QString m_ForecastFallbackFilePath;

    int m_WeatherUpdateIntervalMs = 600000;

    IAppNotificationClient &m_NotificationClient;
};
