#include "WeatherRepository.hpp"

WeatherRepository::WeatherRepository(WeatherService &weatherService, QObject *parent)
    : QObject{parent}, m_WeatherService(weatherService)
{
    connect(
        &m_WeatherService,
        &WeatherService::weatherUpdated,
        this,
        &WeatherRepository::onWeatherUpdated
        );

    connect(
        &m_WeatherService,
        &WeatherService::forecastUpdated,
        this,
        &WeatherRepository::onForecastUpdated
        );

    connect(
        &m_WeatherService,
        &WeatherService::geoLocationsUpdated,
        this,
        &WeatherRepository::onGeoLocationsUpdated
        );

    connect(
        &m_WeatherService,
        &WeatherService::errorOccurred,
        this,
        &WeatherRepository::onErrorOccurred
        );

    connect(
        &m_WeatherService,
        &WeatherService::loadingChanged,
        this,
        &WeatherRepository::onLoadingChanged
        );
}

const WeatherData &WeatherRepository::weather() const
{
    return m_WeatherData;
}

const ForecastData &WeatherRepository::forecast() const
{
    return m_ForecastData;
}

const QList<GeoLocation> &WeatherRepository::geoLocations() const
{
    return m_GeoLocations;
}

bool WeatherRepository::loading() const
{
    return m_Loading;
}

QString WeatherRepository::errorMessage() const
{
    return m_ErrorMessage;
}

void WeatherRepository::updateWeatherForCity(const QString &cityName, const QString &countryCode, int limit)
{
    m_WeatherService.updateWeatherForCity(cityName, countryCode, limit);
}

void WeatherRepository::updateWeatherForCoordinates(double latitude, double longitude)
{
    m_WeatherService.updateWeatherForCoordinates(latitude, longitude);
}

void WeatherRepository::onWeatherUpdated(const WeatherData &weather)
{
    m_WeatherData = weather;

    if (!m_ErrorMessage.isEmpty())
    {
        m_ErrorMessage.clear();
        emit errorMessageChanged(m_ErrorMessage);
    }

    emit weatherChanged(m_WeatherData);
}

void WeatherRepository::onForecastUpdated(const ForecastData &forecast)
{
    m_ForecastData = forecast;

    if (!m_ErrorMessage.isEmpty())
    {
        m_ErrorMessage.clear();
        emit errorMessageChanged(m_ErrorMessage);
    }

    emit forecastChanged(m_ForecastData);
}

void WeatherRepository::onGeoLocationsUpdated(const QList<GeoLocation> &locations)
{
    m_GeoLocations = locations;
    emit geoLocationsChanged(m_GeoLocations);
}

void WeatherRepository::onLoadingChanged(bool loading)
{
    if (m_Loading == loading)
        return;

    m_Loading = loading;
    emit loadingChanged(m_Loading);
}

void WeatherRepository::onErrorOccurred(const QString &message)
{
    m_ErrorMessage = message;
    emit errorMessageChanged(m_ErrorMessage);
}





