#pragma once

#include <QObject>
#include <QString>
#include <QNetworkInformation>

class NetworkStatus final : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool hasNetworkConnection READ hasNetworkConnection NOTIFY networkStatusChanged)
    Q_PROPERTY(bool hasInternetAccess READ hasInternetAccess NOTIFY networkStatusChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY networkStatusChanged)

public:
    explicit NetworkStatus(QObject *parent = nullptr);

    bool hasNetworkConnection() const;
    bool hasInternetAccess() const;
    QString statusText();

signals:
    void networkStatusChanged();

private:
    void updateReachability(QNetworkInformation::Reachability reachability);

    QNetworkInformation::Reachability m_Reachability = QNetworkInformation::Reachability::Unknown;
};
