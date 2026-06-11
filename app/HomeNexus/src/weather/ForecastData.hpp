#pragma once

#include <QString>
#include <QList>
#include <QDateTime>
#include "ForecastEntry.hpp"

struct ForecastData
{
    QDateTime updatedTimestampUtc;
    int timezone = 0.0;
    QList<ForecastEntry> entries;
};
