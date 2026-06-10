#include "ForecastListModel.hpp"
#include <QVariant>
#include <QDateTime>
#include <QHash>
#include <QByteArray>
#include <QDebug>

ForecastListModel::ForecastListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

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
    const QDateTime localDateTime = entry.timeForecastUtc.addSecs(m_Forecast.timezone);

    switch (role)
    {
    case DayTextRole:
        return QLocale().toString(localDateTime, QStringLiteral("dddd"));
    case TimeTextRole:
        return QLocale().toString(localDateTime, QStringLiteral("HH:mm"));
    case TemperatureRole:
        return entry.temperature;
    case PrecipitationProbabilityRole:
        return entry.pop;
    case WeatherIconRole:
        return entry.weatherIcon;
    default:
        return {};
    }
}

QHash<int, QByteArray> ForecastListModel::roleNames() const
{
    QHash<int, QByteArray> mapping
        {
            {ForecastRole::DayTextRole, "dayText"},
            {ForecastRole::TimeTextRole, "timeText"},
            {ForecastRole::TemperatureRole, "temperature"},
            {ForecastRole::PrecipitationProbabilityRole, "precipitationProbability"},
            {ForecastRole::WeatherIconRole, "icon"}
        };
    return mapping;
}

void ForecastListModel::onForecastChanged(const ForecastData &forecast)
{
    beginResetModel();
    m_Forecast = forecast;
    endResetModel();

    if (m_Forecast.entries.empty())
    {
        qDebug() << "ForecastListModel::setForecast: entries are empty";
    }
}
