#include "OpenWeatherClient.hpp"
#include <QStringLiteral>
#include <QUrlQuery>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QByteArray>
#include <QScopeGuard>

OpenWeatherClient::OpenWeatherClient(QNetworkAccessManager &networkManager, AppConfig &config, QObject *parent) :
    QObject(parent), m_NetworkManager(networkManager), m_Config(config)
{

}

void OpenWeatherClient::fetchCurrentWeather(double latitude, double longitude)
{
    QUrl url(m_Config.baseUrl().toString() + QStringLiteral("/data/2.5/weather"));

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), QString::number(latitude));
    query.addQueryItem(QStringLiteral("lon"), QString::number(longitude));
    query.addQueryItem(QStringLiteral("appid"), m_Config.apiKey());
    query.addQueryItem(QStringLiteral("units"), m_Config.units());
    query.addQueryItem(QStringLiteral("lang"), m_Config.language());
    url.setQuery(query);

    if (!url.isValid())
    {
        emit errorOccurred(QStringLiteral("Invalid URL"));
        return;
    }

    QNetworkRequest request(url);
    request.setHeader(
        QNetworkRequest::UserAgentHeader,
        QStringLiteral("HomeNexus/0.1")
        );

    QNetworkReply *reply = m_NetworkManager.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]()
            {
                const auto cleanup = qScopeGuard([reply] {
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

                const QByteArray responseData = reply->readAll();

                emit currentWeatherReceived(responseData);
            });
}

void OpenWeatherClient::fetchForecast(double latitude, double longitude)
{

}
