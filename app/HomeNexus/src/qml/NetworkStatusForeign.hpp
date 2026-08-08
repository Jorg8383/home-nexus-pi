#pragma once

#include <QQmlEngine>

#include "NetworkStatus.hpp"

// Exposes the C++-owned NetworkStatus instance as a QML singleton without QML deps in the service lib.
struct NetworkStatusForeign
{
    Q_GADGET
    QML_FOREIGN(NetworkStatus)
    QML_SINGLETON
    QML_NAMED_ELEMENT(NetworkStatus)

public:
    static NetworkStatus *create(QQmlEngine *, QJSEngine *)
    {
        Q_ASSERT(s_instance);
        QQmlEngine::setObjectOwnership(s_instance, QQmlEngine::CppOwnership);
        return s_instance;
    }

    static void setInstance(NetworkStatus *instance) { s_instance = instance; }

private:
    static inline NetworkStatus *s_instance = nullptr;
};
