#pragma once

#include <QObject>
#include <QString>

#include "AppNotificationTypes.hpp"
#include "AppNotificationCenter.hpp"
#include "IAppNotificationClient.hpp"

class AppNotificationClient : public QObject, public IAppNotificationClient
{
    Q_OBJECT

public:
    explicit AppNotificationClient(AppNotificationCenter &center,
                                   QObject *parent = nullptr);

    void setBannerNotification(
        AppNotificationTypes::Id id,
        const QString &message,
        AppNotificationTypes::Severity severity) override;

    void clearBannerNotification(AppNotificationTypes::Id id) override;

    void showError(const QString &message) override;

private:
    AppNotificationCenter &m_Center;
};
