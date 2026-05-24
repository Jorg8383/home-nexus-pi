#include "OpenWeatherParser.hpp"

CurrentWeather OpenWeatherParser::parseCurrentWeather(const QByteArray& data)
{
    CurrentWeather weather;
    return weather;
}

WeatherForecast OpenWeatherParser::parseForecast(const QByteArray& data)
{
    WeatherForecast forecast;
    return forecast;
}
