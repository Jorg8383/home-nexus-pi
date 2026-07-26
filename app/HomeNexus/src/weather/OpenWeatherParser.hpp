#pragma once

#include <QByteArray>
#include "WeatherData.hpp"
#include "ForecastData.hpp"
#include "GeoLocation.hpp"

class OpenWeatherParser
{
public:
    OpenWeatherParser() = delete; // this class is not meant to be instantiated

    static bool parseWeather(const QByteArray &json, WeatherData &weather);
    static bool parseForecast(const QByteArray &json, ForecastData &forecast);
    static bool parseGeoLocations(const QByteArray &json, QList<GeoLocation> &locations);
};
