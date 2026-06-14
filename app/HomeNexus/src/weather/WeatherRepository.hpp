#pragma once

#include <QObject>
#include <QList>
#include <QString>
#include <QTimer>
#include <QDateTime>

#include "WeatherService.hpp"
#include "ForecastData.hpp"
#include "WeatherData.hpp"
#include "GeoLocation.hpp"
#include "WeatherFallbackProvider.hpp"
#include "IAppConfig.hpp"

class WeatherRepository : public QObject
{
    Q_OBJECT
public:
    explicit WeatherRepository(WeatherService &weatherService,
                               WeatherFallbackProvider &weatherFallback,
                               IAppConfig &config,
                               QObject *parent = nullptr);

    const WeatherData &weather() const;
    const ForecastData &forecast() const;
    const QList<GeoLocation> &geoLocations() const;

    bool loading() const;
    QString errorMessage() const;

    void updateWeatherForCity(const QString &cityName,
                              const QString &countryCode = QString(),
                              int limit = 5);
    void updateWeatherForCoordinates(double latitude, double longitude);
    bool isWeatherStale() const;
    void refreshIfStale();

signals:
    void weatherChanged(const WeatherData &weather);
    void forecastChanged(const ForecastData &forecast);
    void geoLocationsChanged(const QList<GeoLocation> &locations);
    void loadingChanged(bool loading);
    void errorOccurred(const QString &message);
    void warningOccurred(const QString &message);

private slots:
    void onWeatherUpdated(const WeatherData &weather);
    void onForecastUpdated(const ForecastData &forecast);
    void onGeoLocationsUpdated(const QList<GeoLocation> &locations);
    void onUpdateFinished();
    void onFallbackFinished();
    void onServiceErrorOccurred(const QString &message);
    void onFallbackErrorOccurred(const QString &message);
    void onRefreshWeatherData();

private:
    void setLoading(bool loading);
    void loadFallbackData();
    void clearErrorMessage();
    void setErrorMessage(const QString &message);
    bool prepareOnlineUpdate();

private:
    WeatherService &m_WeatherService;
    WeatherFallbackProvider &m_WeatherFallback;
    IAppConfig &m_AppConfig;
    WeatherData m_WeatherData;
    ForecastData m_ForecastData;
    QList<GeoLocation> m_GeoLocations;

    bool m_Loading = false;
    bool m_OnlineUpdateFailed = false;
    QString m_ErrorMessage;

    QTimer m_RefreshTimer;
    QDateTime m_LastUpdateUtc;
    QString m_LastCityName;
    QString m_LastCountryCode;
};
