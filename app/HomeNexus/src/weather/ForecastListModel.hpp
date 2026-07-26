#pragma once

#include <QAbstractListModel>
#include "ForecastData.hpp"

class ForecastListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ForecastRole
    {
        DayTextRole = Qt::UserRole + 1,
        TimeTextRole,
        TemperatureRole,
        PrecipitationProbabilityRole,
        WeatherIconRole
    };

    explicit ForecastListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void setForecast(const ForecastData &forecast);

private:
    ForecastData m_Forecast;
};
