#pragma once

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QUrl>
#include <QStringLiteral>
#include <QNetworkAccessManager>

#include "IAppConfig.hpp"

class OpenWeatherClient : public QObject
{
    Q_OBJECT
public:
    explicit OpenWeatherClient(QNetworkAccessManager &networkManager, const IAppConfig &config, QObject *parent = nullptr);

    void fetchWeather(double latitude, double longitude);
    void fetchForecast(double latitude, double longitude);

private:
    void sendRequest(const QString &endpoint, double latitude, double longitude, std::function<void(const QByteArray&)> onSuccess);

signals:
    void weatherReceived(const QByteArray &json);
    void forecastReceived(const QByteArray &json);
    void errorOccurred(const QString &message);

private:
    QNetworkAccessManager& m_NetworkManager;
    const IAppConfig &m_Config;

};
