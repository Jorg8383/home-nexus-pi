#pragma once

#include <QString>

#include "AppNotificationTypes.hpp"

class IAppNotificationClient
{
public:
    IAppNotificationClient() = default;
    virtual ~IAppNotificationClient() = default;

    IAppNotificationClient(const IAppNotificationClient &) = delete;
    IAppNotificationClient &operator=(const IAppNotificationClient &) = delete;
    IAppNotificationClient(IAppNotificationClient &&) = delete;
    IAppNotificationClient &operator=(IAppNotificationClient &&) = delete;

    virtual void setBannerNotification(AppNotificationTypes::Id id,
                                       AppNotificationTypes::Severity severity,
                                       const QString &message) = 0;

    virtual void clearBannerNotification(AppNotificationTypes::Id id) = 0;

    virtual void showError(const QString &message) = 0;
};
