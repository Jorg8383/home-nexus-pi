#pragma once

#include <QObject>
#include <QString>

#include "WeatherData.hpp"
#include "ForecastData.hpp"

class WeatherFallbackProvider : public QObject
{
    Q_OBJECT
public:
    explicit WeatherFallbackProvider(QObject *parent = nullptr);

    void loadData(const QString &weatherFilePath, const QString &forecastFilePath);

private:
    bool readJsonFile(const QString &filePath, QByteArray &json, QString &errorMessage);

signals:
    void weatherLoaded(const WeatherData &weatherData);
    void forecastLoaded(const ForecastData &forecastData);
    void errorOccurred(const QString &errorMessage);
    void loadingFinished();

};
