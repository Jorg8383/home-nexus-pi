#include "WeatherRepository.hpp"
#include <QDebug>

WeatherRepository::WeatherRepository(WeatherService &weatherService,
                                     WeatherFallbackProvider &weatherFallback,
                                     IAppConfig &config,
                                     IAppNotificationClient &notificationClient,
                                     NetworkStatus &networkStatus,
                                     QObject *parent)
    : QObject{parent}
    , m_WeatherService(weatherService)
    , m_WeatherFallback(weatherFallback)
    , m_AppConfig(config)
    , m_NotificationClient(notificationClient)
    , m_NetworkStatus(networkStatus)
{
    connect(&m_WeatherService,
            &WeatherService::weatherUpdated,
            this,
            &WeatherRepository::onWeatherUpdated
            );

    connect(&m_WeatherService,
            &WeatherService::forecastUpdated,
            this,
            &WeatherRepository::onForecastUpdated
            );

    connect(&m_WeatherService,
            &WeatherService::geoLocationsUpdated,
            this,
            &WeatherRepository::onGeoLocationsUpdated
            );

    connect(&m_WeatherService,
            &WeatherService::errorOccurred,
            this,
            &WeatherRepository::onServiceErrorOccurred
            );

    connect(&m_WeatherService,
            &WeatherService::updateFinished,
            this,
            &WeatherRepository::onUpdateFinished
            );

    connect(&m_WeatherFallback,
            &WeatherFallbackProvider::weatherLoaded,
            this,
            &WeatherRepository::onWeatherUpdated
            );

    connect(&m_WeatherFallback,
            &WeatherFallbackProvider::forecastLoaded,
            this,
            &WeatherRepository::onForecastUpdated
            );

    connect(&m_WeatherFallback,
            &WeatherFallbackProvider::errorOccurred,
            this,
            &WeatherRepository::onFallbackErrorOccurred
            );

    connect(&m_WeatherFallback,
            &WeatherFallbackProvider::loadingFinished,
            this,
            &WeatherRepository::onFallbackFinished
            );

    connect(&m_NetworkStatus,
            &NetworkStatus::networkStatusChanged,
            this,
            &WeatherRepository::onNetworkStatusChanged
            );

    m_LastCityName = m_AppConfig.city();
    m_LastCountryCode = m_AppConfig.countryCode();

    m_RefreshTimer.setInterval(std::chrono::minutes(15));
    m_RefreshTimer.setSingleShot(false);

    connect(&m_RefreshTimer,
            &QTimer::timeout,
            this,
            &WeatherRepository::onRefreshWeatherData
            );

    m_RefreshTimer.start();
}

const WeatherData &WeatherRepository::weather() const
{
    return m_WeatherData;
}

const ForecastData &WeatherRepository::forecast() const
{
    return m_ForecastData;
}

const QList<GeoLocation> &WeatherRepository::geoLocations() const
{
    return m_GeoLocations;
}

bool WeatherRepository::loading() const
{
    return m_Loading;
}

QString WeatherRepository::errorMessage() const
{
    return m_ErrorMessage;
}

void WeatherRepository::updateWeatherForCity(const QString &cityName, const QString &countryCode, int limit)
{
    if (!prepareOnlineUpdate())
        return;

    m_WeatherService.updateWeatherForCity(cityName, countryCode, limit);
}

void WeatherRepository::updateWeatherForCoordinates(double latitude, double longitude)
{
    if (!prepareOnlineUpdate())
        return;

    m_WeatherService.updateWeatherForCoordinates(latitude, longitude);
}

void WeatherRepository::refreshIfStale()
{
    if (m_LastCityName.isEmpty())
    {
        m_NotificationClient.setBannerNotification(AppNotificationTypes::Id::WeatherInvalidLocation,
                                                   AppNotificationTypes::Severity::Warning,
                                                   QStringLiteral("No location definied, cannot update weather data.")
                                                   );
        qDebug() << "WeatherRepository::refreshIfStale -> last city name is empty";
        return;
    }

    if (!isWeatherStale())
        return;

    updateWeatherForCity(m_LastCityName, m_LastCountryCode, 5);
}

void WeatherRepository::onWeatherUpdated(const WeatherData &weather)
{
    m_WeatherData = weather;
    emit weatherChanged(m_WeatherData);
}

void WeatherRepository::onForecastUpdated(const ForecastData &forecast)
{
    m_ForecastData = forecast;
    emit forecastChanged(m_ForecastData);
}

void WeatherRepository::onGeoLocationsUpdated(const QList<GeoLocation> &locations)
{
    m_GeoLocations = locations;
    emit geoLocationsChanged(m_GeoLocations);
}

void WeatherRepository::onUpdateFinished()
{
    if (m_OnlineUpdateFailed)
    {
        loadFallbackData();
        return;
    }
    setLoading(false);
    m_LastUpdateUtc = QDateTime::currentDateTimeUtc();
    qDebug() << "WeatherRepository::onUpdateFinished -> " << QDateTime::currentDateTime().toString();

    m_NotificationClient.clearBannerNotification(AppNotificationTypes::Id::WeatherUpdateFailed);
    m_NotificationClient.clearBannerNotification(AppNotificationTypes::Id::WeatherInvalidLocation);
    m_NotificationClient.clearBannerNotification(AppNotificationTypes::Id::WeatherMissingApiKey);
}

void WeatherRepository::onFallbackFinished()
{
    setLoading(false);
}

void WeatherRepository::onServiceErrorOccurred(const QString &message)
{
    m_OnlineUpdateFailed = true;

    m_NotificationClient.setBannerNotification(AppNotificationTypes::Id::WeatherUpdateFailed,
                                               AppNotificationTypes::Severity::Warning,
                                               message);

    qDebug() << "WeatherRepository::onServiceErrorOccurred -> " << message;
}

void WeatherRepository::onFallbackErrorOccurred(const QString &message)
{
    m_NotificationClient.showError(QStringLiteral("Could not load weather data: %1").arg(message));
    qDebug() << message;
}

void WeatherRepository::onRefreshWeatherData()
{
    if (m_LastCityName.isEmpty())
    {
        m_NotificationClient.setBannerNotification(AppNotificationTypes::Id::WeatherInvalidLocation,
                                                   AppNotificationTypes::Severity::Warning,
                                                   QStringLiteral("No location is defined, cannot update weather data.")
                                                   );
        qDebug() << "WeatherRepository::refreshIfStale: last city name is empty";
        return;
    }

    updateWeatherForCity(m_LastCityName, m_LastCountryCode, 5);
    qDebug() << "WeatherRepository::onRefreshWeatherData -> " << QDateTime::currentDateTime().toString();
}

void WeatherRepository::onNetworkStatusChanged()
{
    m_NetworkStatusOnline = m_NetworkStatus.hasInternetAccess();
}

void WeatherRepository::setLoading(bool loading)
{
    if (m_Loading == loading)
        return;

    m_Loading = loading;
    emit loadingChanged(m_Loading);
}

void WeatherRepository::loadFallbackData()
{
    m_WeatherFallback.loadData(m_AppConfig.weatherFallbackFilePath(),
                               m_AppConfig.forecastFallbackFilePath());
}

bool WeatherRepository::prepareOnlineUpdate()
{
    m_OnlineUpdateFailed = false;
    setLoading(true);

    if (!m_NetworkStatusOnline)
    {
        m_NotificationClient.setBannerNotification(AppNotificationTypes::Id::NetworkOffline,
                                                   AppNotificationTypes::Severity::Warning,
                                                   QStringLiteral("No internet connection. Showing fallback weather data.")
                                                   );
        qDebug() << "WeatherRepository::prepareOnlineUpdate -> No internet connection";
        loadFallbackData();
        return false;
    }

    if (!m_AppConfig.hasApiKey())
    {
        m_NotificationClient.setBannerNotification(AppNotificationTypes::Id::WeatherMissingApiKey,
                                                   AppNotificationTypes::Severity::Warning,
                                                   QStringLiteral("OpenWeather API key is missing. Showing fallback weather data.")
                                                   );
        qDebug() << "WeatherRepository::prepareOnlineUpdate -> OpenWeather API key is missing, loading fallback data";
        loadFallbackData();
        return false;
    }

    return true;
}

bool WeatherRepository::isWeatherStale() const
{
    return !m_LastUpdateUtc.isValid() || m_LastUpdateUtc.secsTo(QDateTime::currentDateTimeUtc()) > 3 * 60;
}





