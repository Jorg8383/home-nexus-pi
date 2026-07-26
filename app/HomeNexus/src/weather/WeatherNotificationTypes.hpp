#pragma once

#include <QObject>

namespace WeatherNotificationTypes
{
    Q_NAMESPACE

    enum class Id
    {
        MissingApiKey = 0,
        UsingFallbackData,
        WeatherRequestFailed,
        ForecastRequestFailed,
        StaleCachedData
    };
    Q_ENUM_NS(Id) // make enum visible to Qt's meta-object-system

    enum class Severity
    {
        Info = 0,
        Warning = 1
    };
    Q_ENUM_NS(Severity)
} // namespace WeatherNotificationTypes

// Register the enum types with Qt's meta-type system so they can be
// stored in QVariant and safely passed through queued signal/slot connections.
Q_DECLARE_METATYPE(WeatherNotificationTypes::Id)
Q_DECLARE_METATYPE(WeatherNotificationTypes::Severity)
