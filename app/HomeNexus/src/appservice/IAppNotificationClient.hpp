#pragma once

#include <QString>

#include "AppNotificationTypes.hpp"

class IAppNotificationClient
{
public:
    virtual ~IAppNotificationClient() = default;

    virtual void setBannerNotification(
        AppNotificationTypes::Id id,
        AppNotificationTypes::Severity severity,
        const QString &message) = 0;

    virtual void clearBannerNotification(AppNotificationTypes::Id id) = 0;

    virtual void showError(const QString &message) = 0;
};
