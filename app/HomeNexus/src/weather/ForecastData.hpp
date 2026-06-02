#pragma once

#include <QString>
#include <QVector>
#include <QDateTime>
#include "ForecastEntry.hpp"

struct ForecastData
{
    QVector<ForecastEntry> entries;
};
