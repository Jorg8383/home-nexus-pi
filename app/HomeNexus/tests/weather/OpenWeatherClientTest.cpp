#include <QtTest/QtTest>

#include "OpenWeatherClient.hpp"

class OpenWeatherClientTest : public QObject
{
    Q_OBJECT

private slots:
    void canBeCreatedWithoutParent();
    void canBeCreatedWithParent();
};

void OpenWeatherClientTest::canBeCreatedWithoutParent()
{
    OpenWeatherClient client;

    QCOMPARE(client.parent(), nullptr);
}

void OpenWeatherClientTest::canBeCreatedWithParent()
{
    QObject parent;
    OpenWeatherClient client(&parent);

    QCOMPARE(client.parent(), &parent);
}

QTEST_MAIN(OpenWeatherClientTest)

#include "OpenWeatherClientTest.moc"
