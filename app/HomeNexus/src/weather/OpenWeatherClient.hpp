#pragma once

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QNetworkAccessManager>
#include "AppConfig.hpp"

class OpenWeatherClient : public QObject
{
    Q_OBJECT
public:
    explicit OpenWeatherClient(QNetworkAccessManager &networkManager, AppConfig &config, QObject *parent = nullptr);

    void fetchCurrentWeather(double latitude, double longitude);
    void fetchForecast(double latitude, double longitude);

signals:
    void currentWeatherReceived(const QByteArray &json);
    void forecastReceived(const QByteArray &json);
    void errorOccurred(const QString &message);

private:
    QNetworkAccessManager& m_NetworkManager;
    AppConfig& m_Config;
};
