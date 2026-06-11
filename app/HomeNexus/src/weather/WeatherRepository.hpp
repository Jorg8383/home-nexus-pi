#pragma once

#include <QObject>
#include <QList>
#include <QString>

#include "WeatherService.hpp"
#include "ForecastData.hpp"
#include "WeatherData.hpp"
#include "GeoLocation.hpp"

class WeatherRepository : public QObject
{
    Q_OBJECT
public:
    explicit WeatherRepository(WeatherService &weatherService, QObject *parent = nullptr);

    const WeatherData &weather() const;
    const ForecastData &forecast() const;
    const QList<GeoLocation> &geoLocations() const;

    bool loading() const;
    QString errorMessage() const;

    void updateWeatherForCity(const QString &cityName, const QString &countryCode = QString(), int limit = 5);
    void updateWeatherForCoordinates(double latitude, double longitude);

signals:
    void weatherChanged(const WeatherData &weather);
    void forecastChanged(const ForecastData &forecast);
    void geoLocationsChanged(const QList<GeoLocation> &locations);
    void loadingChanged(bool loading);
    void errorMessageChanged(const QString &message);


private slots:
    void onWeatherUpdated(const WeatherData &weather);
    void onForecastUpdated(const ForecastData &forecast);
    void onGeoLocationsUpdated(const QList<GeoLocation> &locations);
    void onLoadingChanged(bool loading);
    void onErrorOccurred(const QString &message);

private:
    WeatherService &m_WeatherService;
    WeatherData m_WeatherData;
    ForecastData m_ForecastData;
    QList<GeoLocation> m_GeoLocations;

    bool m_Loading = false;
    QString m_ErrorMessage;
};
