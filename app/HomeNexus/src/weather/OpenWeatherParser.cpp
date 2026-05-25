#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QDebug>
#include <QDateTime>
#include <QTimeZone>
#include "OpenWeatherParser.hpp"
#include "json/JsonReader.hpp"

using HomeNexusUtils::JsonReader;

bool OpenWeatherParser::parseCurrentWeather(const QByteArray &json, CurrentWeather &weather)
{
    const QString context{"CurrentWeather"};
    CurrentWeather tempWeather;
    QJsonParseError parseError;

    const QJsonDocument doc = QJsonDocument::fromJson(json, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        qWarning() << "JSON current weather parse error:" << parseError.errorString();
        return false;
    }
    if (!doc.isObject())
    {
        qWarning() << "Expected JSON root in current weather to be an object";
        return false;
    }

    QJsonObject rootObj = doc.object();

    QJsonObject coordObj;
    if (!JsonReader::readObject(rootObj, QStringLiteral("coord"), coordObj, context))
        return false;

    if (!JsonReader::readDouble(coordObj, QStringLiteral("lat"), tempWeather.latitude, context + QStringLiteral(".coord")))
        return false;

    if (!JsonReader::readDouble(coordObj, QStringLiteral("lon"), tempWeather.longitude, context + QStringLiteral(".coord")))
        return false;

    QJsonArray weatherArr;
    if (!JsonReader::readArray(rootObj, QStringLiteral("weather"), weatherArr, context))
        return false;

    QJsonObject weatherObj;
    if (!JsonReader::readObjectAt(weatherArr, 0, weatherObj, context + QStringLiteral(".weather")))
        return false;

    if (!JsonReader::readString(weatherObj, QStringLiteral("main"), tempWeather.weatherMain, context + QStringLiteral(".weather[0]")))
        return false;

    if (!JsonReader::readString(weatherObj, QStringLiteral("description"), tempWeather.weatherDescription, context + QStringLiteral(".weather[0]")))
        return false;

    if (!JsonReader::readString(weatherObj, QStringLiteral("icon"), tempWeather.weatherIcon, context + QStringLiteral(".weather[0]")))
        return false;

    QJsonObject mainObj;
    if (!JsonReader::readObject(rootObj, QStringLiteral("main"), mainObj, context))
        return false;

    if (!JsonReader::readDouble(mainObj, QStringLiteral("temp"), tempWeather.temperature, context + QStringLiteral(".main")))
        return false;

    if (!JsonReader::readInt(mainObj, QStringLiteral("pressure"), tempWeather.pressure, context + QStringLiteral(".main")))
        return false;

    if (!JsonReader::readInt(mainObj, QStringLiteral("humidity"), tempWeather.humidity, context + QStringLiteral(".main")))
        return false;

    QJsonObject windObj;
    if (!JsonReader::readObject(rootObj, QStringLiteral("wind"), windObj, context))
        return false;

    if (!JsonReader::readDouble(windObj, QStringLiteral("speed"), tempWeather.windSpeed, context + QStringLiteral(".wind")))
        return false;

    if (!JsonReader::readInt(windObj, QStringLiteral("deg"), tempWeather.windDegrees, context + QStringLiteral(".wind")))
        return false;

    QJsonObject sysObj;
    if (!JsonReader::readObject(rootObj, QStringLiteral("sys"), sysObj, context))
        return false;

    qint64 sunriseUtc = 0;
    if (!JsonReader::readInt64(sysObj, QStringLiteral("sunrise"), sunriseUtc, context + QStringLiteral(".sys")))
        return false;
    tempWeather.sunrise = QDateTime::fromSecsSinceEpoch(sunriseUtc, QTimeZone::UTC);

    qint64 sunsetUtc = 0;
    if (!JsonReader::readInt64(sysObj, QStringLiteral("sunset"), sunsetUtc, context + QStringLiteral(".sys")))
        return false;
    tempWeather.sunset = QDateTime::fromSecsSinceEpoch(sunsetUtc, QTimeZone::UTC);

    if (!JsonReader::readInt(rootObj, QStringLiteral("timezone"), tempWeather.timezone, context))
        return false;

    qint64 dt = 0;
    if (!JsonReader::readInt64(rootObj, QStringLiteral("dt"), dt, context))
        return false;
    tempWeather.timestamp = QDateTime::fromSecsSinceEpoch(dt, QTimeZone::UTC);

    weather = tempWeather;
    return true;
}

bool OpenWeatherParser::parseForecast(const QByteArray &json, WeatherForecast &forecast)
{
    return true;
}
