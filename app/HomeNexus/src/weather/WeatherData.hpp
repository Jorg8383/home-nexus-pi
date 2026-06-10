#pragma once

#include <QDateTime>
#include <QString>

struct WeatherData
{
    // Longitude of the location
    double longitude = 0.0;
    // Latitude of the location
    double latitude = 0.0;
    // Group of weather parameters (Rain, Snow, Clouds etc.)
    QString weatherMain;
    // Weather condition within the group
    QString weatherDescription;
    // Weather icon id
    QString weatherIcon;
    // Temperature. Unit: Celsius
    double temperature = 0.0;
    // Atmospheric pressure on the sea level, hPa
    int pressure = 0.0;
    // Humidity, %
    int humidity = 0.0;
    // Wind speed. Unit Default: meter/sec
    double windSpeed = 0.0;
    // Wind direction, degrees (meteorological)
    int windDegrees = 0;
    // Time of data calculation, unix, UTC
    QDateTime timestampUtc;
    //  Shift in seconds from UTC
    int timezoneOffsetSeconds = 0.0;
    // Sunrise time, unix, UTC
    QDateTime sunriseUtc;
    // Sunset time, unix, UTC
    QDateTime sunsetUtc;
};
