#pragma once

#include <QObject>
#include <QString>

#include "AppNotificationTypes.hpp"
#include "AppNotificationCenter.hpp"


class AppNotificationClient : public QObject
{
    Q_OBJECT

public:
    explicit AppNotificationClient(AppNotificationCenter &center,
                                   QObject *parent = nullptr);

    void setBannerNotification(
        AppNotificationTypes::Id id,
        const QString &message,
        AppNotificationTypes::Severity severity);

    void clearBannerNotification(AppNotificationTypes::Id id);

    void showError(const QString &message);

private:
    AppNotificationCenter &m_Center;
};
