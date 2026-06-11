#include "WeatherService.hpp"
#include "OpenWeatherParser.hpp"
#include <algorithm>

WeatherService::WeatherService(GeoCodingClient &geoCodingClient, OpenWeatherClient &weatherClient, QObject *parent)
    : QObject(parent), m_GeoCodingClient(geoCodingClient), m_WeatherClient(weatherClient)
{
    connect(
        &m_GeoCodingClient,
        &GeoCodingClient::geoLocationsReceived,
        this,
        &WeatherService::onGeoLocationsReceived
        );

    connect(
        &m_GeoCodingClient,
        &GeoCodingClient::errorOccurred,
        this,
        &WeatherService::onClientErrorOccurred
        );

    connect(
        &m_WeatherClient,
        &OpenWeatherClient::weatherReceived,
        this,
        &WeatherService::onWeatherJsonReceived
        );

    connect(
        &m_WeatherClient,
        &OpenWeatherClient::forecastReceived,
        this,
        &WeatherService::onForecastJsonReceived
        );

    connect(
        &m_WeatherClient,
        &OpenWeatherClient::errorOccurred,
        this,
        &WeatherService::onClientErrorOccurred
        );
}

void WeatherService::updateWeatherForCity(const QString &cityName, const QString &countryCode, int limit)
{
    const QString city = cityName.trimmed();
    const QString country = countryCode.trimmed().toUpper();
    const int clampedLimit = std::clamp(limit, 1, 5);

    if (city.isEmpty())
    {
        emit errorOccurred(QStringLiteral("City name must not be empty"));
        return;
    }

    setLoading(true);

    m_GeoCodingClient.fetchGeoLocations(city, country, clampedLimit);
}

void WeatherService::updateWeatherForCoordinates(const double latitude, const double longitude)
{
    m_PendingRequests = 2;
    setLoading(true);

    m_WeatherClient.fetchWeather(latitude, longitude);
    m_WeatherClient.fetchForecast(latitude, longitude);
}

void WeatherService::onGeoLocationsReceived(const QByteArray &json)
{
    if (json.trimmed().isEmpty())
    {
        setLoading(false);
        emit errorOccurred(QStringLiteral("JSON geocoding response is empty"));
        return;
    }

    QList<GeoLocation> locations;

    if (!OpenWeatherParser::parseGeoLocations(json, locations))
    {
        setLoading(false);
        emit errorOccurred(QStringLiteral("Failed to parse geo locations"));
        return;
    }

    emit geoLocationsUpdated(locations);

    // TODO: let the user later choose the location if multiple locations are returned
    const GeoLocation &location = locations.first();
    m_CityName = location.cityName;

    updateWeatherForCoordinates(location.latitude, location.longitude);
}

void WeatherService::onWeatherJsonReceived(const QByteArray &json)
{
    WeatherData weather;

    if (!OpenWeatherParser::parseCurrentWeather(json, weather))
    {
        emit errorOccurred(QStringLiteral("Failed to parse current weather data"));
        requestFinished();
        return;
    }

    weather.cityName = m_CityName;

    emit weatherUpdated(weather);
    requestFinished();
}

void WeatherService::onForecastJsonReceived(const QByteArray &json)
{
    ForecastData forecast;

    if (!OpenWeatherParser::parseForecast(json, forecast))
    {
        emit errorOccurred(QStringLiteral("Failed to parse forecast data"));
        requestFinished();
        return;
    }

    emit forecastUpdated(forecast);
    requestFinished();
}

void WeatherService::onClientErrorOccurred(const QString &message)
{
    m_PendingRequests = 0;
    setLoading(false);
    emit errorOccurred(message);
}

void WeatherService::setLoading(bool loading)
{
    if (m_Loading == loading)
        return;

    m_Loading = loading;
    emit loadingChanged(m_Loading);
}

void WeatherService::requestFinished()
{
    if (m_PendingRequests > 0)
        m_PendingRequests--;

    if (m_PendingRequests == 0)
        setLoading(false);
}
