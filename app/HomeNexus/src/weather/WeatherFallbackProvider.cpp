#include "WeatherFallbackProvider.hpp"
#include <QFile>
#include <QStringLiteral>
#include <QByteArray>
#include <QScopeGuard>
#include "OpenWeatherParser.hpp"

WeatherFallbackProvider::WeatherFallbackProvider(QObject *parent)
    : QObject{parent}
{}

void WeatherFallbackProvider::loadData(const QString &weatherFilePath, const QString &forecastFilePath)
{
    QString errorMessage;

    auto cleanup = qScopeGuard([this] { emit loadingFinished(); });

    QByteArray weatherRawData;
    if (!readJsonFile(weatherFilePath, weatherRawData, errorMessage))
    {
        emit errorOccurred(errorMessage);
        return;
    }

    WeatherData weatherData;
    if (!OpenWeatherParser::parseWeather(weatherRawData, weatherData))
    {
        emit errorOccurred(QStringLiteral("Failed to parse JSON file: %1").arg(weatherFilePath));
        return;
    }

    QByteArray forecastRawData;
    if (!readJsonFile(forecastFilePath, forecastRawData, errorMessage))
    {
        emit errorOccurred(errorMessage);
        return;
    }

    ForecastData forecastData;
    if (!OpenWeatherParser::parseForecast(forecastRawData, forecastData))
    {
        emit errorOccurred(QStringLiteral("Failed to parse JSON file: %1").arg(forecastFilePath));
        return;
    }

    emit weatherLoaded(weatherData);
    emit forecastLoaded(forecastData);
}

bool WeatherFallbackProvider::readJsonFile(const QString &filePath, QByteArray &json, QString &errorMessage)
{
    QFile file(filePath);

    if (!file.exists())
    {
        errorMessage = QStringLiteral("Fallback JSON file does not exist: %1").arg(filePath);
        return false;
    }

    if (!file.open(QIODeviceBase::ReadOnly))
    {
        errorMessage = QStringLiteral("Could not open fallback JSON file: %1").arg(filePath);
        return false;
    }

    json = file.readAll();

    if (json.isEmpty())
    {
        errorMessage = QStringLiteral("Fallback JSON file is empty: %1").arg(filePath);
        return false;
    }

    return true;
}
