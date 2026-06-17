#include "AppNotificationCenter.hpp"
#include <algorithm>

AppNotificationCenter::AppNotificationCenter(QObject *parent)
    : QObject(parent)
{
}

bool AppNotificationCenter::hasBannerNotification() const
{
    return m_CurrentBannerId != AppNotificationTypes::Id::Unknown;
}

QString AppNotificationCenter::bannerMessage() const
{
    return m_BannerMessage;
}

int AppNotificationCenter::bannerSeverity() const
{
    return static_cast<int>(m_BannerSeverity);
}

bool AppNotificationCenter::hasError() const
{
    return m_HasError;
}

QString AppNotificationCenter::errorMessage() const
{
    return m_ErrorMessage;
}

void AppNotificationCenter::setBannerNotification(
    AppNotificationTypes::Id id,
    const QString &message,
    AppNotificationTypes::Severity severity)
{
    if (id == AppNotificationTypes::Id::Unknown)
        return;

    if (message.trimmed().isEmpty())
    {
        clearBannerNotification(id);
        return;
    }

    m_BannerNotifications.insert(id, BannerNotification{message, severity});

    // Move this notification to the end, meaning "latest".
    m_BannerOrder.removeAll(id);
    m_BannerOrder.append(id);

    updateCurrentBannerNotification();
}

void AppNotificationCenter::clearBannerNotification(AppNotificationTypes::Id id)
{
    if (id == AppNotificationTypes::Id::Unknown)
        return;

    m_BannerNotifications.remove(id);
    m_BannerOrder.removeAll(id);

    updateCurrentBannerNotification();
}

void AppNotificationCenter::showError(const QString &message)
{
    if (message.trimmed().isEmpty())
        return;

    if (m_HasError && m_ErrorMessage == message)
        return;

    m_HasError = true;
    m_ErrorMessage = message;

    emit errorNotificationChanged();
}

void AppNotificationCenter::clearError()
{
    if (!m_HasError && m_ErrorMessage.isEmpty())
        return;

    m_HasError = false;
    m_ErrorMessage.clear();

    emit errorNotificationChanged();
}

void AppNotificationCenter::updateCurrentBannerNotification()
{
    // Remove stale IDs from the order list.
    m_BannerOrder.erase(
        std::remove_if(
            m_BannerOrder.begin(),
            m_BannerOrder.end(),
            [this](AppNotificationTypes::Id id)
            {
                return !m_BannerNotifications.contains(id);
            }),
        m_BannerOrder.end());

    AppNotificationTypes::Id newId = AppNotificationTypes::Id::Unknown;

    // First priority: latest active warning.
    for (auto it = m_BannerOrder.crbegin(); it != m_BannerOrder.crend(); ++it)
    {
        const auto id = *it;
        const auto notification = m_BannerNotifications.value(id);

        if (notification.severity == AppNotificationTypes::Severity::Warning)
        {
            newId = id;
            break;
        }
    }

    // Second priority: latest active info.
    if (newId == AppNotificationTypes::Id::Unknown)
    {
        for (auto it = m_BannerOrder.crbegin(); it != m_BannerOrder.crend(); ++it)
        {
            const auto id = *it;
            const auto notification = m_BannerNotifications.value(id);

            if (notification.severity == AppNotificationTypes::Severity::Info)
            {
                newId = id;
                break;
            }
        }
    }

    QString newMessage;
    AppNotificationTypes::Severity newSeverity = AppNotificationTypes::Severity::Info;

    if (newId != AppNotificationTypes::Id::Unknown)
    {
        const auto notification = m_BannerNotifications.value(newId);
        newMessage = notification.message;
        newSeverity = notification.severity;
    }

    if (m_CurrentBannerId == newId
        && m_BannerMessage == newMessage
        && m_BannerSeverity == newSeverity)
    {
        return;
    }

    m_CurrentBannerId = newId;
    m_BannerMessage = newMessage;
    m_BannerSeverity = newSeverity;

    emit bannerNotificationChanged();
}
