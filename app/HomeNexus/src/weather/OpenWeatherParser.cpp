#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QDebug>
#include <QDateTime>
#include <QTimeZone>
#include "OpenWeatherParser.hpp"
#include "JsonReader.hpp"
#include "ForecastEntry.hpp"

using HomeNexusUtils::JsonReader;

bool OpenWeatherParser::parseCurrentWeather(const QByteArray &json, WeatherData &weather)
{
    const QString context{"CurrentWeather"};
    WeatherData tempWeather;
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

bool OpenWeatherParser::parseForecast(const QByteArray &json, ForecastData &forecast)
{
    const QString context{"Forecast"};
    ForecastData tempForecast;
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

    QJsonArray listArr;
    if (!JsonReader::readArray(rootObj, QStringLiteral("list"), listArr, context))
        return false;

    for (int i = 0; i < listArr.size(); ++i)
    {
        ForecastEntry forecastEntry;

        QJsonObject listItem;
        if (!JsonReader::readObjectAt(listArr, i, listItem, context + QStringLiteral(".list")))
            return false;

        qint64 dt = 0;
        if (!JsonReader::readInt64(listItem, QStringLiteral("dt"), dt, context + QStringLiteral(".list[%1]").arg(i)))
            return false;
        forecastEntry.timeForecastUtc = QDateTime::fromSecsSinceEpoch(dt, QTimeZone::UTC);

        if (!JsonReader::readDouble(listItem, QStringLiteral("pop"), forecastEntry.pop , context + QStringLiteral(".list[%1]").arg(i)))
            return false;

        QJsonObject mainItem;
        if (!JsonReader::readObject(listItem, QStringLiteral("main"), mainItem, context + QStringLiteral(".list[%1]").arg(i)))
            return false;

        if (!JsonReader::readDouble(mainItem, QStringLiteral("temp"), forecastEntry.temperature, context + QStringLiteral(".list[%1]")))
            return false;

        if (!JsonReader::readInt(mainItem, QStringLiteral("humidity"), forecastEntry.humidity, context + QStringLiteral(".list[%1]")))
            return false;

        QJsonArray weatherArr;
        if (!JsonReader::readArray(listItem, QStringLiteral("weather"), weatherArr, context + QStringLiteral(".list[%1]").arg(i)))
            return false;

        QJsonObject weatherItem;
        if (!JsonReader::readObjectAt(weatherArr, 0, weatherItem, context + QStringLiteral(".list[%1]").arg(i)))
            return false;

        if (!JsonReader::readString(weatherItem, QStringLiteral("main"), forecastEntry.weatherMain, context + QStringLiteral(".list[%1].weather").arg(i)))
            return false;

        if (!JsonReader::readString(weatherItem, QStringLiteral("description"), forecastEntry.weatherDescription, context + QStringLiteral(".list[%1].weather").arg(i)))
            return false;

        if (!JsonReader::readString(weatherItem, QStringLiteral("icon"), forecastEntry.weatherIcon, context + QStringLiteral(".list[%1].weather").arg(i)))
            return false;

        tempForecast.entries.append(forecastEntry);
    }

    forecast = tempForecast;
    return true;
}

bool OpenWeatherParser::parseGeoLocations(const QByteArray &json, QList<GeoLocation> &locations)
{
    QList<GeoLocation> parsedLocations;
    QJsonParseError parseError;

    const QJsonDocument doc = QJsonDocument::fromJson(json, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        qWarning() << "JSON geocoding parse error:" << parseError.errorString();
        return false;
    }
    if (!doc.isArray())
    {
        qWarning() << "Expected JSON root in direct geocoding to be an array";
        return false;
    }

    QJsonArray rootArr;
    rootArr = doc.array();

    for (int i = 0; i < rootArr.size(); ++i)
    {
        GeoLocation geoLocation;
        QJsonObject locationObj;
        if (!JsonReader::readObjectAt(rootArr, i, locationObj))
            return false;

        if (!JsonReader::readString(locationObj, QStringLiteral("name"), geoLocation.cityName, QStringLiteral("location[%1]").arg(i)))
            return false;

        if (!JsonReader::readString(locationObj, QStringLiteral("country"), geoLocation.country, QStringLiteral("location[%1]").arg(i)))
            return false;

        if (!JsonReader::readString(locationObj, QStringLiteral("state"), geoLocation.state, QStringLiteral("location[%1]").arg(i)))
            return false;

        if (!JsonReader::readDouble(locationObj, QStringLiteral("lat"), geoLocation.latitude, QStringLiteral("location[%1]").arg(i)))
            return false;

        if (!JsonReader::readDouble(locationObj, QStringLiteral("lon"), geoLocation.longitude, QStringLiteral("location[%1]").arg(i)))
            return false;

        parsedLocations.append(geoLocation);
    }

    locations = parsedLocations;
    return true;
}
