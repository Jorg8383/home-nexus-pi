#pragma once

#include <QObject>
#include <QList>

#include "GeoCodingClient.hpp"
#include "OpenWeatherClient.hpp"
#include "WeatherData.hpp"
#include "ForecastData.hpp"
#include "GeoLocation.hpp"

class WeatherService : public QObject
{
    Q_OBJECT
public:
    explicit WeatherService(GeoCodingClient &geoCodingClient,
                            OpenWeatherClient &weatherClient,
                            QObject *parent = nullptr);

    void updateWeatherForCity(const QString &cityName,
                              const QString &countryCode = QString(),
                              int limit = 5);

    void updateWeatherForCoordinates(const double latitude, const double longitude);

signals:
    void weatherUpdated(const WeatherData &weather);
    void forecastUpdated(const ForecastData &forecast);
    void geoLocationsUpdated(const QList<GeoLocation> &locations);
    void errorOccurred(const QString &message);
    void infoOccurred(const QString &message);
    void updateFinished();

private slots:
    void onGeoLocationsReceived(const QByteArray &json);
    void onWeatherJsonReceived(const QByteArray &json);
    void onForecastJsonReceived(const QByteArray &json);
    void onClientErrorOccurred(const QString &message);

private:
    void requestFinished();

private:
    GeoCodingClient &m_GeoCodingClient;
    OpenWeatherClient &m_WeatherClient;
    QString m_CityName;
    QString m_Country;

    int m_PendingRequests = 0;
};
