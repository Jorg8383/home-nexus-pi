#pragma once

#include <QObject>

namespace AppNotificationTypes
{
Q_NAMESPACE

enum class Id
{
    Unknown = 0,

    WeatherMissingApiKey,
    WeatherUsingFallbackData,
    WeatherRequestFailed,
    WeatherForecastRequestFailed
};
Q_ENUM_NS(Id)

enum class Severity
{
    Info = 0,
    Warning = 1
};
Q_ENUM_NS(Severity)

// Hash function for Id enum class to be used in Qt's hash-based containers
inline size_t qHash(Id id, size_t seed = 0) noexcept
{
    using Underlying = std::underlying_type_t<Id>;
    return ::qHash(static_cast<Underlying>(id), seed);
}
}
