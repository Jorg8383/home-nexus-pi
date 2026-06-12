#include <QtTest/QtTest>

#include "OpenWeatherClient.hpp"
#include "AppConfig.hpp"

class OpenWeatherClientTest : public QObject
{
    Q_OBJECT

public:
    explicit OpenWeatherClientTest(QObject * parent = nullptr);

private slots:
    void canBeCreatedWithoutParent();
    void canBeCreatedWithParent();

private:
    QNetworkAccessManager m_NetworkAccessManager;
    AppConfig m_Config;
};

OpenWeatherClientTest::OpenWeatherClientTest(QObject *parent)
    : QObject(parent)
    , m_NetworkAccessManager(this)
    , m_Config(QStringLiteral("testconfig.ini"))
{

}

void OpenWeatherClientTest::canBeCreatedWithoutParent()
{
    OpenWeatherClient client(m_NetworkAccessManager, m_Config);

    QCOMPARE(client.parent(), nullptr);
}

void OpenWeatherClientTest::canBeCreatedWithParent()
{
    QObject parent;
    OpenWeatherClient client(m_NetworkAccessManager, m_Config, &parent);

    QCOMPARE(client.parent(), &parent);
}

QTEST_MAIN(OpenWeatherClientTest)

#include "OpenWeatherClientTest.moc"
