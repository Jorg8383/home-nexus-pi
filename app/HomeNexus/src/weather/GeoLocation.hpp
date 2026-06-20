#pragma once

#include <QString>

struct GeoLocation
{
    QString cityName;
    QString country;
    double latitude = 0.0;
    double longitude = 0.0;
};
