#include <QtTest/QTest>

#include <QFile>
#include <QByteArray>
#include <QString>
#include <QDateTime>
#include <QList>

#include "OpenWeatherParser.hpp"


class OpenWeatherParserTest : public QObject
{
    Q_OBJECT

private:
    QByteArray loadTestFile(const QString& fileName) const;

    QByteArray m_jsonWeather;
    QByteArray m_jsonForecast;
    QByteArray m_jsonGeoLocations;

    // Qt test cases are declared as private slots
private slots:
    void initTestCase();
    void parsesValidCurrentWeatherData();
    void parsesValidForecastData();
    void parsesValidGeoLocations();
};

//---------------------------------------------------------------------------
QByteArray OpenWeatherParserTest::loadTestFile(const QString& fileName) const
{
    QString filePath = QFINDTESTDATA(QStringLiteral("testdata/") + fileName);
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Failed to open test file: %s", qPrintable(file.fileName()));
    }

    return file.readAll();
}

//---------------------------------------------------------------------------
void OpenWeatherParserTest::initTestCase()
{
    m_jsonWeather = loadTestFile(QStringLiteral("CurrentWeather.json"));
    m_jsonForecast = loadTestFile(QStringLiteral("Forecast.json"));
    m_jsonGeoLocations = loadTestFile(QStringLiteral("GeoLocations.json"));
}

//---------------------------------------------------------------------------
void OpenWeatherParserTest::parsesValidCurrentWeatherData()
{
    WeatherData result;
    const auto success = OpenWeatherParser::parseWeather(m_jsonWeather, result);

    QVERIFY(success);

    QCOMPARE(result.longitude, 9.5639);
    QCOMPARE(result.latitude, 48.3739);
    QCOMPARE(result.weatherMain, QStringLiteral("Clear"));
    QCOMPARE(result.weatherDescription, QStringLiteral("clear sky"));
    QCOMPARE(result.weatherIcon, QStringLiteral("01d"));
    QCOMPARE(result.temperature, 20.17);
    QCOMPARE(result.pressure, 1026);
    QCOMPARE(result.humidity, 100);
    QCOMPARE(result.windSpeed, 1.89);
    QCOMPARE(result.windDegrees, 67);
    QCOMPARE(result.timestampUtc, QDateTime::fromSecsSinceEpoch(1779559664, QTimeZone::UTC));
    QCOMPARE(result.timezoneOffsetSeconds, 7200);
    QCOMPARE(result.sunriseUtc, QDateTime::fromSecsSinceEpoch(1779507171, QTimeZone::UTC));
    QCOMPARE(result.sunsetUtc, QDateTime::fromSecsSinceEpoch(1779563065, QTimeZone::UTC));

}

//---------------------------------------------------------------------------
void OpenWeatherParserTest::parsesValidForecastData()
{
    ForecastData result;
    const auto success = OpenWeatherParser::parseForecast(m_jsonForecast, result);

    QVERIFY(success);

    QCOMPARE(result.entries.size(), 40);

    const ForecastEntry& firstEntry = result.entries.at(0);

    QCOMPARE(firstEntry.weatherMain, QStringLiteral("Clouds"));
    QCOMPARE(firstEntry.weatherDescription, QStringLiteral("overcast clouds"));
    QCOMPARE(firstEntry.weatherIcon, QStringLiteral("04n"));

    QCOMPARE(firstEntry.temperature, 21.48);
    QCOMPARE(firstEntry.humidity, 88);
    QCOMPARE(firstEntry.pop, 0.0);

    QCOMPARE(firstEntry.forecastTimestampUtc, QDateTime(QDate(2026, 5, 24), QTime(21, 0, 0), QTimeZone::UTC));

    const ForecastEntry& rainEntry = result.entries.at(21);

    QCOMPARE(rainEntry.weatherMain, QStringLiteral("Rain"));
    QCOMPARE(rainEntry.weatherDescription, QStringLiteral("light rain"));
    QCOMPARE(rainEntry.weatherIcon, QStringLiteral("10d"));

    QCOMPARE(rainEntry.temperature, 23.05);
    QCOMPARE(rainEntry.humidity, 63);
    QCOMPARE(rainEntry.pop, 1.0);

    QCOMPARE(rainEntry.forecastTimestampUtc, QDateTime(QDate(2026, 5, 27), QTime(12, 0, 0), QTimeZone::UTC));

    QCOMPARE(result.timezone, 7200);
}

//---------------------------------------------------------------------------
void OpenWeatherParserTest::parsesValidGeoLocations()
{
    QList<GeoLocation> locations;
    const auto success = OpenWeatherParser::parseGeoLocations(m_jsonGeoLocations, locations);

    QVERIFY(success);

    QCOMPARE(locations.size(), 2);

    const GeoLocation& firstLocation = locations.at(0);
    QCOMPARE(firstLocation.cityName, QStringLiteral("Münsingen"));
    QCOMPARE(firstLocation.country, QStringLiteral("CH"));
    QCOMPARE(firstLocation.state, QStringLiteral("Bern"));
    QCOMPARE(firstLocation.latitude, 46.8739775);
    QCOMPARE(firstLocation.longitude, 7.5631943);

    const GeoLocation& secondLocation = locations.at(1);
    QCOMPARE(secondLocation.cityName, QStringLiteral("Münsingen"));
    QCOMPARE(secondLocation.country, QStringLiteral("DE"));
    QCOMPARE(secondLocation.state, QStringLiteral("Baden-Württemberg"));
    QCOMPARE(secondLocation.latitude, 48.4128592);
    QCOMPARE(secondLocation.longitude, 9.4947894);
}
//---------------------------------------------------------------------------

QTEST_MAIN(OpenWeatherParserTest)

#include "OpenWeatherParserTest.moc"
