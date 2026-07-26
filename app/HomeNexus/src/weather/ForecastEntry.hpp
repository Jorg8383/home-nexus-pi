#pragma once

#include <QDateTime>
#include <QString>

struct ForecastEntry
{
    // Group of weather parameters (Rain, Snow, Clouds etc.)
    QString weatherMain;
    // Weather condition within the group
    QString weatherDescription;
    // Weather icon id
    QString weatherIcon;
    // Temperature. Metric: Celsius
    double temperature = 0.0;
    // Humidity, %
    int humidity = 0;
    // Probability of precipitation. The values of the parameter vary between 0 and 1, where 0 is
    // equal to 0%, 1 is equal to 100%
    double pop = 0.0;
    // Date and time of data forecasted
    QDateTime forecastTimestampUtc;
};
