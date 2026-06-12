#include "GeoCodingClient.hpp"

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QScopeGuard>
#include <QByteArray>
#include <algorithm> // std::clamp

GeoCodingClient::GeoCodingClient(QNetworkAccessManager &networkManager, const IAppConfig &config, QObject *parent)
    : QObject(parent), m_NetworkManager(networkManager), m_Config(config)
{

}

void GeoCodingClient::fetchGeoLocations(const QString &cityName, const QString &countryCode, int limit)
{

    QString locationQuery = cityName.trimmed();

    if (locationQuery.isEmpty())
    {
        emit errorOccurred(QStringLiteral("City name must not be emtpy"));
        return;
    }
    if (!countryCode.trimmed().isEmpty())
    {
        locationQuery += QStringLiteral(",") + countryCode.trimmed();
    }

    limit = std::clamp(limit, 1, 5);

    QUrl url(QStringLiteral("https://api.openweathermap.org/geo/1.0/direct"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("q"), locationQuery);
    query.addQueryItem(QStringLiteral("limit"), QString::number(limit));
    query.addQueryItem(QStringLiteral("appid"), m_Config.apiKey());
    url.setQuery(query);

    if (!url.isValid())
    {
        emit errorOccurred(QStringLiteral("Invalid URL : %1").arg(url.toString()));
        return;
    }

    QNetworkRequest request(url);
    request.setRawHeader(
        QByteArrayLiteral("Accept"),
        QByteArrayLiteral("application/json")
        );

    QNetworkReply *reply = m_NetworkManager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply]()
                     {
                         const auto cleanup = qScopeGuard([reply]
                                                          {
                                                              reply->deleteLater();
                                                          });

                         if (reply->error() != QNetworkReply::NoError)
                         {
                             emit errorOccurred(reply->errorString());
                             return;
                         }

                         const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

                         if (statusCode < 200 || statusCode >= 300)
                         {
                             emit errorOccurred(QStringLiteral("HTTP error: %1").arg(statusCode));
                             return;
                         }

                         const QByteArray payload = reply->readAll();

                         emit geoLocationsReceived(payload);
                     });
}
