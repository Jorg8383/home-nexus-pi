#pragma once

#include <QString>

class IAppConfig
{
public:
    IAppConfig() = default;
    virtual ~IAppConfig() = default;

    IAppConfig(const IAppConfig &) = delete;
    IAppConfig &operator=(const IAppConfig &) = delete;
    IAppConfig(IAppConfig &&) = delete;
    IAppConfig &operator=(IAppConfig &&) = delete;

    virtual QString apiKey() const = 0;
    virtual QString units() const = 0;
    virtual QString language() const = 0;
    virtual QString city() const = 0;
    virtual QString countryCode() const = 0;

    virtual bool hasApiKey() const = 0;
    virtual bool configFileExists() const = 0;
    virtual bool onlineWeatherEnabled() const = 0;

    virtual QString weatherFallbackFilePath() const = 0;
    virtual QString forecastFallbackFilePath() const = 0;

    virtual int weatherUpdateIntervalMs() const = 0;

    virtual void saveWeatherLocation(const QString &city, const QString &country) = 0;
};
