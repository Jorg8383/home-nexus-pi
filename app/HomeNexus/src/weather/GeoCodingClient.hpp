#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QNetworkAccessManager>
#include "AppConfig.hpp"

class GeoCodingClient : public QObject
{
    Q_OBJECT
public:
    explicit GeoCodingClient(QNetworkAccessManager &networkManager, AppConfig &config, QObject *parent = nullptr);

    void fetchGeoLocations(const QString& cityName, const QString& countryCode, int limit = 5);

signals:
    void geoLocationsReceived(const QByteArray& json);
    void errorOccurred(const QString& message);

private:
    QNetworkAccessManager& m_NetworkManager;
    AppConfig& m_Config;
};
