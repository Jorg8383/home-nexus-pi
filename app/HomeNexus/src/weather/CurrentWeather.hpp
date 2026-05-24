#pragma once

#include <QDateTime>
#include <QString>

struct CurrentWeather
{
    // Longitude of the location
    double longitude = 0.0;
    // Latitude of the location
    double latitude = 0.0;
    // City name
    QString cityName;
    // Country code (GB, JP, etc.)
    QString countryCode;
    // Group of weather parameters (Rain, Snow, Clouds etc.)
    QString weatherMain;
    // Weather condition within the group
    QString weatherDescription;
    // Weather icon id
    QString weatherIcon;
    // Temperature. Unit: Celsius
    double temperature = 0.0;
    // Atmospheric pressure on the sea level, hPa
    int pressure = 0;
    // Humidity, %
    int humidity = 0;
    // Wind speed. Unit Default: meter/sec
    double windSpeed = 0.0;
    // Wind direction, degrees (meteorological)
    double windDegrees = 0.0;
    // Time of data calculation, unix, UTC
    QDateTime timestampUtc;
    //  Shift in seconds from UTC
    int timezone = 0;
    // Sunrise time, unix, UTC
    QDateTime sunriseUtc;
    // Sunset time, unix, UTC
    QDateTime sunsetUtc;
};
