#pragma once

#include <QString>
#include <QVector>
#include <QDateTime>
#include "ForecastEntry.hpp"

struct ForecastData
{
    QDateTime updatedTimestampUtc;
    QVector<ForecastEntry> entries;
};
