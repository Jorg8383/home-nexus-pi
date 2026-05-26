#pragma once

#pragma once

#include <QString>
#include <QUrl>

class IOpenWeatherConfig
{
public:
    virtual ~IOpenWeatherConfig() = default;

    virtual bool hasApiKey() const = 0;
    virtual QString apiKey() const = 0;
    virtual QUrl baseUrl() const = 0;
    virtual QString units() const = 0;
    virtual QString language() const = 0;
    virtual QString city() const = 0;
    virtual QString countryCode() const = 0;

    virtual int requestTimeoutMs() const = 0;
    virtual int currentWeatherUpdateIntervalMs() const = 0;
    virtual int forecastUpdateIntervalMs() const = 0;
};
