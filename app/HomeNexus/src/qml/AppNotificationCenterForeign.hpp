#pragma once

#include <QQmlEngine>

#include "AppNotificationCenter.hpp"

// Exposes the C++-owned AppNotificationCenter instance as a QML singleton without QML deps in the service lib.
struct AppNotificationCenterForeign
{
    Q_GADGET
    QML_FOREIGN(AppNotificationCenter)
    QML_SINGLETON
    QML_NAMED_ELEMENT(AppNotificationCenter)

public:
    static AppNotificationCenter *create(QQmlEngine *, QJSEngine *)
    {
        Q_ASSERT(s_instance);
        QQmlEngine::setObjectOwnership(s_instance, QQmlEngine::CppOwnership);
        return s_instance;
    }

    static void setInstance(AppNotificationCenter *instance) { s_instance = instance; }

private:
    static inline AppNotificationCenter *s_instance = nullptr;
};
