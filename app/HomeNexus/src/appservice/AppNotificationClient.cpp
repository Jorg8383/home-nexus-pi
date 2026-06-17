#include "AppNotificationClient.hpp"
#include <QMetaObject>

AppNotificationClient::AppNotificationClient(AppNotificationCenter &center,
                                             QObject *parent)
    : QObject(parent),
    m_Center(center)
{
}

void AppNotificationClient::setBannerNotification(
    AppNotificationTypes::Id id,
    AppNotificationTypes::Severity severity,
    const QString &message)
{
    QMetaObject::invokeMethod(
        &m_Center,
        [&center = m_Center, id, message, severity]()
        {
            center.setBannerNotification(id, message, severity);
        },
        Qt::QueuedConnection);
}

void AppNotificationClient::clearBannerNotification(AppNotificationTypes::Id id)
{
    QMetaObject::invokeMethod(
        &m_Center,
        [&center = m_Center, id]()
        {
            center.clearBannerNotification(id);
        },
        Qt::QueuedConnection);
}

void AppNotificationClient::showError(const QString &message)
{
    QMetaObject::invokeMethod(
        &m_Center,
        [&center = m_Center, message]()
        {
            center.showError(message);
        },
        Qt::QueuedConnection);
}
