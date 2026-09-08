#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QNetworkAccessManager>

#include "IAppConfig.hpp"
#include "WeatherConstants.hpp"

class GeoCodingClient : public QObject
{
    Q_OBJECT
public:
    explicit GeoCodingClient(QNetworkAccessManager &networkManager,
                             const IAppConfig &config,
                             QObject *parent = nullptr);

    void fetchGeoLocations(const QString &cityName,
                           const QString &countryCode,
                           int limit = WeatherConstants::MaxLocationResults);

signals:
    void geoLocationsReceived(const QByteArray &json);
    void errorOccurred(const QString &message);

private:
    QNetworkAccessManager &m_NetworkManager;
    const IAppConfig &m_Config;
};
