#pragma once

#include <QByteArray>
#include "CurrentWeather.hpp"
#include "WeatherForecast.hpp"

class OpenWeatherParser
{
public:
    OpenWeatherParser() = delete; // this class is not meant to be instantiated

    static CurrentWeather parseCurrentWeather(const QByteArray& data);
    static WeatherForecast parseForecast(const QByteArray& data);

};
