#include "OpenWeatherClient.hpp"
#include <QStringLiteral>
#include <QUrlQuery>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QByteArray>
#include <QScopeGuard>

#include "IAppConfig.hpp"


OpenWeatherClient::OpenWeatherClient(QNetworkAccessManager &networkManager, const IAppConfig &config, QObject *parent) :
    QObject(parent), m_NetworkManager(networkManager), m_Config(config)
{
}

void OpenWeatherClient::fetchWeather(double latitude, double longitude)
{
    QString urlString(QStringLiteral("https://api.openweathermap.org/data/2.5/weather"));
    sendRequest(
        urlString,
        latitude,
        longitude,
        [this](const QByteArray &json)
        {
            emit weatherReceived(json);
        }
        );
}

void OpenWeatherClient::fetchForecast(double latitude, double longitude)
{
    QString urlString(QStringLiteral("https://api.openweathermap.org/data/2.5/forecast"));
    sendRequest(
        urlString,
        latitude,
        longitude,
        [this](const QByteArray &json)
        {
            emit forecastReceived(json);
        }
        );
}

void OpenWeatherClient::sendRequest(const QString &endpoint, double latitude, double longitude, std::function<void (const QByteArray &)> onSuccess)
{
    QUrl url(endpoint);

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), QString::number(latitude));
    query.addQueryItem(QStringLiteral("lon"), QString::number(longitude));
    query.addQueryItem(QStringLiteral("appid"), m_Config.apiKey());
    query.addQueryItem(QStringLiteral("units"), m_Config.units());
    query.addQueryItem(QStringLiteral("lang"), m_Config.language());
    url.setQuery(query);

    if (!url.isValid())
    {
        emit errorOccurred(QStringLiteral("Invalid URL: %1").arg(url.toString()));
        return;
    }

    QNetworkRequest request(url);
    request.setRawHeader(
        QByteArrayLiteral("Accept"),
        QByteArrayLiteral("application/json")
        );

    QNetworkReply *reply = m_NetworkManager.get(request);

    /* Important note:
    The lambda function requires a copy [=] of 'reply' instead of a reference [&]
    because the method fetchCurrentWeather will have gone out of scope by the time
    the signal QNetworkReplay::finished is emitted and invokes the lambda function.
    */
    connect(reply, &QNetworkReply::finished, this, [this, reply, onSuccess]()
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

                const QByteArray responseData = reply->readAll();

                if (onSuccess)
                    onSuccess(responseData);
            });
}
