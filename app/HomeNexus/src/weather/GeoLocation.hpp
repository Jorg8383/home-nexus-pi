#pragma once

#include <QString>

struct GeoLocation
{
    QString cityName;
    QString country;
    QString state;
    double latitude = 0.0;
    double longitude = 0.0;
};
