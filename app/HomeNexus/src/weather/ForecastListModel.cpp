#include "ForecastListModel.hpp"
#include <QVariant>
#include <QDateTime>
#include <QHash>
#include <QByteArray>
#include <QDebug>

ForecastListModel::ForecastListModel(QObject *parent) : QAbstractListModel(parent) {}

int ForecastListModel::rowCount(const QModelIndex &parent) const
{
    // This is a flat list model. It has one row per forecast entry
    if (parent.isValid())
        return 0;

    return static_cast<int>(m_Forecast.entries.size());
}

QVariant ForecastListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    if (index.row() < 0 || index.row() >= m_Forecast.entries.size())
        return {};

    const ForecastEntry &entry = m_Forecast.entries.at(index.row());
    const QDateTime localDateTime = entry.forecastTimestampUtc;

    switch (static_cast<ForecastRole>(role))
    {
        case ForecastRole::DayTextRole:
            return QLocale().toString(localDateTime, QStringLiteral("dddd"));
        case ForecastRole::TimeTextRole:
            return QLocale().toString(localDateTime, QStringLiteral("HH:mm"));
        case ForecastRole::TemperatureRole:
            return entry.temperature;
        case ForecastRole::PrecipitationProbabilityRole:
            return entry.pop;
        case ForecastRole::WeatherIconRole:
            return entry.weatherIcon;
        default:
            return {};
    }
}

QHash<int, QByteArray> ForecastListModel::roleNames() const
{
    return {
        {static_cast<int>(ForecastRole::DayTextRole), "dayText"},
        {static_cast<int>(ForecastRole::TimeTextRole), "timeText"},
        {static_cast<int>(ForecastRole::TemperatureRole), "temperature"},
        {static_cast<int>(ForecastRole::PrecipitationProbabilityRole), "precipitationProbability"},
        {static_cast<int>(ForecastRole::WeatherIconRole), "weatherIcon"},
    };
}

void ForecastListModel::setForecast(const ForecastData &forecast)
{
    beginResetModel();
    m_Forecast = forecast;
    endResetModel();

    if (m_Forecast.entries.empty())
    {
        qDebug() << "ForecastListModel::setForecast: entries are empty";
    }
}
