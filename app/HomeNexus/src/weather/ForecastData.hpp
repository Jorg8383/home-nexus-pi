#pragma once

#include <QString>
#include <QList>
#include <QDateTime>
#include "ForecastEntry.hpp"

struct ForecastData
{
    QDateTime updatedTimestampUtc;
    QList<ForecastEntry> entries;
};
