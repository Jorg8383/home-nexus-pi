#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>

#include "WeatherRepository.hpp"
#include "ForecastListModel.hpp"

class WeatherViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString city READ city NOTIFY weatherChanged)
    Q_PROPERTY(QString weatherMain READ weatherMain NOTIFY weatherChanged)
    Q_PROPERTY(QString weatherDescription READ weatherDescription NOTIFY weatherChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon NOTIFY weatherChanged)

    Q_PROPERTY(double temperature READ temperature NOTIFY weatherChanged)
    Q_PROPERTY(int humidity READ humidity NOTIFY weatherChanged)
    Q_PROPERTY(int pressure READ pressure NOTIFY weatherChanged)

    Q_PROPERTY(double windSpeed READ windSpeed NOTIFY weatherChanged)
    Q_PROPERTY(int windDegrees READ windDegrees NOTIFY weatherChanged)

    Q_PROPERTY(QString sunriseText READ sunriseText NOTIFY weatherChanged)
    Q_PROPERTY(QString sunsetText READ sunsetText NOTIFY weatherChanged)

    Q_PROPERTY(QString lastUpdatedText READ lastUpdatedText NOTIFY weatherChanged)

    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorChanged)
    Q_PROPERTY(bool hasError READ hasError NOTIFY errorChanged)

    Q_PROPERTY(QString warningMessage READ warningMessage NOTIFY warningChanged)
    Q_PROPERTY(bool hasWarning READ hasWarning NOTIFY warningChanged)

    Q_PROPERTY(ForecastListModel *forecastModel READ forecastModel CONSTANT)

public:
    explicit WeatherViewModel(WeatherRepository &repository, QObject *parent = nullptr);

    QString city() const;
    QString weatherMain() const;
    QString weatherDescription() const;
    QString weatherIcon() const;

    double temperature() const;
    int humidity() const;
    int pressure() const;

    double windSpeed() const;
    int windDegrees() const;

    QString sunriseText() const;
    QString sunsetText() const;

    QString lastUpdatedText() const;

    bool loading() const;

    QString errorMessage() const;
    bool hasError() const;

    QString warningMessage() const;
    bool hasWarning() const;

    ForecastListModel *forecastModel();

    // Q_INVOKABLE void updateWeatherForCity(
    //         const QString &cityName,
    //         const QString &countryCode = QString(),
    //         int limit = 5
    //         );

    void updateWeatherForCity(
        const QString &cityName,
        const QString &countryCode = QString(),
        int limit = 5
        );

signals:
    void weatherChanged();
    void loadingChanged();
    void errorChanged();
    void warningChanged();

private slots:
    void onWeatherChanged(const WeatherData &weatherData);
    void onLoadingChanged(bool loading);
    void onErrorOccurred(const QString &message);
    void onWarningOccurred(const QString &message);

private:
    QString formatTime(const QDateTime &dateTime) const;

private:
    WeatherRepository &m_Repository;
    ForecastListModel m_ForecastListModel;

    WeatherData m_WeatherData;

    bool m_Loading = false;
    QString m_ErrorMessage;
    QString m_WarningMessage;
};
