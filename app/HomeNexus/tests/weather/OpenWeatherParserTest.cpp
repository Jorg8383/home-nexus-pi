#include <QtTest/QTest>

#include <QFile>
#include <QByteArray>
#include <QString>

#include "OpenWeatherParser.hpp"


class OpenWeatherParserTest : public QObject
{
    Q_OBJECT

private:
    QByteArray loadTestFile(const QString& fileName) const;

    QByteArray m_jsonWeather;
    QByteArray m_jsonForecast;

    // Qt test cases are declared as private slots
private slots:
    void initTestCase();
    void parsesValidCurrentWeatherData();
    void parsesVaidForecastData();
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
}

//---------------------------------------------------------------------------
void OpenWeatherParserTest::parsesValidCurrentWeatherData()
{
    const auto result = OpenWeatherParser::parseCurrentWeather(m_jsonWeather);

    QCOMPARE(result.longitude, 9.5639);
    QCOMPARE(result.latitude, 48.3739);
    QCOMPARE(result.cityName, QStringLiteral("Mehrstetten"));
    QCOMPARE(result.countryCode, QStringLiteral("DE"));
    QCOMPARE(result.weatherMain, QStringLiteral("Clear"));
    QCOMPARE(result.weatherDescription, QStringLiteral("clear sky"));
    QCOMPARE(result.weatherIcon, QStringLiteral("01d"));
    QCOMPARE(result.temperature, 20.17);
    QCOMPARE(result.pressure, 1026);
    QCOMPARE(result.humidity, 100);
    QCOMPARE(result.windSpeed, 1.89);
    QCOMPARE(result.windDegrees, 67);
    QCOMPARE(result.timestampUtc, QDateTime::fromSecsSinceEpoch(1779559664, QTimeZone::UTC));
    QCOMPARE(result.timezone, 7200);
    QCOMPARE(result.sunriseUtc, QDateTime::fromSecsSinceEpoch(1779507171, QTimeZone::UTC));
    QCOMPARE(result.sunsetUtc, QDateTime::fromSecsSinceEpoch(1779563065, QTimeZone::UTC));

}

//---------------------------------------------------------------------------
void OpenWeatherParserTest::parsesVaidForecastData()
{
    const auto result = OpenWeatherParser::parseForecast(m_jsonForecast);

    QCOMPARE(result.cityName, QStringLiteral("Mehrstetten"));
    QCOMPARE(result.countryCode, QStringLiteral("DE"));

    QCOMPARE(result.entries.size(), 40);

    const ForecastEntry& firstEntry = result.entries.at(0);

    QCOMPARE(firstEntry.weatherMain, QStringLiteral("Clouds"));
    QCOMPARE(firstEntry.weatherDescription, QStringLiteral("overcast clouds"));
    QCOMPARE(firstEntry.weatherIcon, QStringLiteral("04n"));

    QCOMPARE(firstEntry.temperature, 21.48);
    QCOMPARE(firstEntry.humidity, 88);
    QCOMPARE(firstEntry.pop, 0.0);
    QCOMPARE(firstEntry.windSpeed, 1.96);
    QCOMPARE(firstEntry.windDegrees, 14.0);

    QCOMPARE(firstEntry.timeForecast, QTime(21, 0, 0));
    QCOMPARE(firstEntry.timezone, 7200);

    const ForecastEntry& rainEntry = result.entries.at(21);

    QCOMPARE(rainEntry.weatherMain, QStringLiteral("Rain"));
    QCOMPARE(rainEntry.weatherDescription, QStringLiteral("light rain"));
    QCOMPARE(rainEntry.weatherIcon, QStringLiteral("10d"));

    QCOMPARE(rainEntry.temperature, 23.05);
    QCOMPARE(rainEntry.humidity, 63);
    QCOMPARE(rainEntry.pop, 1.0);
    QCOMPARE(rainEntry.windSpeed, 3.03);
    QCOMPARE(rainEntry.windDegrees, 15.0);

    QCOMPARE(rainEntry.timeForecast, QTime(12, 0, 0));
    QCOMPARE(rainEntry.timezone, 7200);
}
//---------------------------------------------------------------------------

QTEST_MAIN(OpenWeatherParserTest)

#include "OpenWeatherParserTest.moc"
