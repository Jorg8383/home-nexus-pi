#include <QtTest/QtTest>

#include "OpenWeatherClient.hpp"
#include "AppConfig.hpp"
#include "AppNotificationCenter.hpp"
#include "AppNotificationClient.hpp"

class OpenWeatherClientTest : public QObject
{
    Q_OBJECT

public:
    explicit OpenWeatherClientTest(QObject *parent = nullptr);

private slots:
    void canBeCreatedWithoutParent();
    void canBeCreatedWithParent();

private:
    QNetworkAccessManager m_NetworkAccessManager;
    AppConfig m_Config;
    AppNotificationCenter m_NotificationCenter;
    AppNotificationClient m_NotificationClient;
};

OpenWeatherClientTest::OpenWeatherClientTest(QObject *parent)
    : QObject(parent), m_NetworkAccessManager(this), m_NotificationCenter(this),
      m_NotificationClient(m_NotificationCenter, this),
      m_Config(QStringLiteral("testconfig.ini"), m_NotificationClient)
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
