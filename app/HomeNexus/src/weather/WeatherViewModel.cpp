#include "WeatherViewModel.hpp"
#include <QLocale>

WeatherViewModel::WeatherViewModel(WeatherRepository &repository, QObject *parent)
    : QObject{parent}, m_Repository(repository),
      m_ForecastListModel(this), // passing 'this' to the constructor of ForecastListModel
      m_WeatherData(repository.weather()), m_Loading(repository.loading())
{
    connect(&m_Repository,
            &WeatherRepository::weatherChanged,
            this,
            &WeatherViewModel::onWeatherChanged);

    connect(&m_Repository,
            &WeatherRepository::forecastChanged,
            &m_ForecastListModel,
            &ForecastListModel::setForecast);

    connect(&m_Repository,
            &WeatherRepository::loadingChanged,
            this,
            &WeatherViewModel::onLoadingChanged);
}

QString WeatherViewModel::city() const
{
    return m_WeatherData.cityName;
}

QString WeatherViewModel::weatherMain() const
{
    return m_WeatherData.weatherMain;
}

QString WeatherViewModel::weatherDescription() const
{
    return m_WeatherData.weatherDescription;
}

QString WeatherViewModel::weatherIcon() const
{
    return m_WeatherData.weatherIcon;
}

double WeatherViewModel::temperature() const
{
    return m_WeatherData.temperature;
}

int WeatherViewModel::humidity() const
{
    return m_WeatherData.humidity;
}

int WeatherViewModel::pressure() const
{
    return m_WeatherData.pressure;
}

double WeatherViewModel::windSpeed() const
{
    return m_WeatherData.windSpeed;
}

int WeatherViewModel::windDegrees() const
{
    return m_WeatherData.windDegrees;
}

QString WeatherViewModel::sunriseText() const
{
    return formatTime(m_WeatherData.sunriseUtc);
}

QString WeatherViewModel::sunsetText() const
{
    return formatTime(m_WeatherData.sunsetUtc);
}

QString WeatherViewModel::lastUpdatedText() const
{
    if (!m_WeatherData.timestampUtc.isValid())
        return {};

    const QDateTime localDateTime =
        m_WeatherData.timestampUtc.addSecs(m_WeatherData.timezoneOffsetSeconds);

    return QLocale().toString(localDateTime, QStringLiteral("dddd, HH:mm"));
}

bool WeatherViewModel::loading() const
{
    return m_Loading;
}

ForecastListModel *WeatherViewModel::forecastModel()
{
    return &m_ForecastListModel;
}

void WeatherViewModel::updateWeatherForCity(const QString &cityName,
                                            const QString &countryCode,
                                            int limit)
{
    m_Repository.updateWeatherForCity(cityName, countryCode, limit);
}

void WeatherViewModel::refreshIfNeeded()
{
    m_Repository.refreshIfStale();
}

void WeatherViewModel::onWeatherChanged(const WeatherData &weatherData)
{
    m_WeatherData = weatherData;
    emit weatherChanged();
}

void WeatherViewModel::onLoadingChanged(bool loading)
{
    if (m_Loading != loading)
    {
        m_Loading = loading;
        emit loadingChanged();
    }
}

QString WeatherViewModel::formatTime(const QDateTime &dateTime) const
{
    if (!dateTime.isValid())
        return {};

    const QDateTime localDateTime = dateTime.addSecs(m_WeatherData.timezoneOffsetSeconds);

    return QLocale().toString(localDateTime, QStringLiteral("HH:mm"));
}
