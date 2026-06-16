#pragma once

#include <QObject>
#include <QHash>
#include <QList>
#include <QString>

#include "AppNotificationTypes.hpp"

class AppNotificationCenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool hasBannerNotification READ hasBannerNotification NOTIFY bannerNotificationChanged)
    Q_PROPERTY(QString bannerMessage READ bannerMessage NOTIFY bannerNotificationChanged)
    Q_PROPERTY(int bannerSeverity READ bannerSeverity NOTIFY bannerNotificationChanged)

    Q_PROPERTY(bool hasError READ hasError NOTIFY errorNotificationChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorNotificationChanged)

public:
    explicit AppNotificationCenter(QObject *parent = nullptr);

    bool hasBannerNotification() const;
    QString bannerMessage() const;
    int bannerSeverity() const;

    bool hasError() const;
    QString errorMessage() const;

    void setBannerNotification(AppNotificationTypes::Id id,
                               const QString &message,
                               AppNotificationTypes::Severity severity);

    void clearBannerNotification(AppNotificationTypes::Id id);

    void showError(const QString &message);

    Q_INVOKABLE void clearError();

signals:
    void bannerNotificationChanged();
    void errorNotificationChanged();

private:
    struct BannerNotification
    {
        QString message;
        AppNotificationTypes::Severity severity = AppNotificationTypes::Severity::Info;
    };

    void updateCurrentBannerNotification();

    QHash<AppNotificationTypes::Id, BannerNotification> m_BannerNotifications;
    QList<AppNotificationTypes::Id> m_BannerOrder;

    AppNotificationTypes::Id m_CurrentBannerId = AppNotificationTypes::Id::Unknown;
    QString m_BannerMessage;
    AppNotificationTypes::Severity m_BannerSeverity = AppNotificationTypes::Severity::Info;

    bool m_HasError = false;
    QString m_ErrorMessage;
};
