#pragma once

#include <QString>
#include <QVector>
#include <QDateTime>
#include "ForecastEntry.hpp"

struct WeatherForecast
{
    QVector<ForecastEntry> entries;
};
