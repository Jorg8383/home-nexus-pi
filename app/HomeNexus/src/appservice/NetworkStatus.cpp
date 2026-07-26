#include "NetworkStatus.hpp"

#include <QDebug>

NetworkStatus::NetworkStatus(QObject *parent) : QObject{parent}
{
    if (!QNetworkInformation::loadDefaultBackend())
    {
        qWarning()
            << "NetworkStatus::NetworkStatus -> QNetworkInformation backend could not be loaded.";
        return;
    }

    QNetworkInformation *networkInformation = QNetworkInformation::instance();

    if (!networkInformation)
    {
        qWarning()
            << "NetworkStatus::NetworkStatus -> QNetworkInformation instance is not available.";
        return;
    }

    connect(networkInformation,
            &QNetworkInformation::reachabilityChanged,
            this,
            &NetworkStatus::updateReachability);

    updateReachability(networkInformation->reachability());
}

bool NetworkStatus::hasNetworkConnection() const
{
    return m_Reachability == QNetworkInformation::Reachability::Local ||
           m_Reachability == QNetworkInformation::Reachability::Site ||
           m_Reachability == QNetworkInformation::Reachability::Online;
}

bool NetworkStatus::hasInternetAccess() const
{
    return m_Reachability == QNetworkInformation::Reachability::Online;
}

QString NetworkStatus::statusText()
{
    QString text(QStringLiteral(""));

    switch (m_Reachability)
    {
        case QNetworkInformation::Reachability::Unknown:
            text = QStringLiteral("Network status unknown");

        case QNetworkInformation::Reachability::Disconnected:
            text = QStringLiteral("No network connection");

        case QNetworkInformation::Reachability::Local:
            text = QStringLiteral("Connected to local network only");

        case QNetworkInformation::Reachability::Site:
            text = QStringLiteral("Connected to local site or intranet");

        case QNetworkInformation::Reachability::Online:
            text = QStringLiteral("Online");
    }

    return text;
}

void NetworkStatus::updateReachability(QNetworkInformation::Reachability reachability)
{
    if (m_Reachability == reachability)
        return;

    m_Reachability = reachability;

    qDebug() << "NetworkStatus::updateReachability -> Status changed to:" << statusText();

    emit networkStatusChanged();
}
