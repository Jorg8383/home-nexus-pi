#pragma once

#include <QString>
#include <QVector>
#include <QDateTime>
#include "ForecastEntry.hpp"

struct WeatherForecast
{
    QString cityName;
    QString countryCode;

    QDateTime updatedTimestampUtc;

    QVector<ForecastEntry> entries;
};
