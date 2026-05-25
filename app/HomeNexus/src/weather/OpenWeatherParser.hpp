#pragma once

#include <QByteArray>
#include "CurrentWeather.hpp"
#include "WeatherForecast.hpp"

class OpenWeatherParser
{
public:
    OpenWeatherParser() = delete; // this class is not meant to be instantiated

    static bool parseCurrentWeather(const QByteArray& json, CurrentWeather& weather);
    static bool parseForecast(const QByteArray& json, WeatherForecast& forecast);

};
