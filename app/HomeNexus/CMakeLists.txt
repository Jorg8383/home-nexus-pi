cmake_minimum_required(VERSION 3.16)

project(HomeNexus VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Quick
    Core
    Network
    Test
)

qt_standard_project_setup(REQUIRES 6.8)

# Enable CTest integration, which also creates the BUILD_TESTING option
include(CTest)

add_subdirectory(src)

# Tests
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

qt_add_executable(appHomeNexus
    main.cpp
)

set_source_files_properties(ui/theme/Style.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

set_source_files_properties(
    ui/utils/WeatherIconMapper.js
    ui/utils/ClimateIconMapper.js
    ui/utils/AirQualityMapper.js
    PROPERTIES
        QT_QML_SKIP_QMLDIR_ENTRY TRUE
)

qt_add_qml_module(appHomeNexus
    URI HomeNexus
    VERSION 1.0
    QML_FILES
        Main.qml
        ui/components/AppHeader.qml
        ui/components/AppTabButton.qml
        ui/components/AppButton.qml
        ui/components/AppCard.qml
        ui/components/AppMetricCard.qml
        ui/components/ClimateSensorOverview.qml
        ui/components/AppTopicCard.qml
        ui/components/AppWarningBanner.qml
        ui/data/StaticForecastModel.qml
        ui/data/StaticWeatherModel.qml
        ui/pages/home/HomeRootPage.qml
        ui/pages/climate/ClimateGreenhousePage.qml
        ui/pages/climate/ClimateRootPage.qml
        ui/pages/climate/ClimateIndoorPage.qml
        ui/pages/climate/ClimateWeatherPage.qml
        ui/pages/climate/ForecastDelegate.qml
        ui/pages/climate/WeatherAppCard.qml
        ui/pages/climate/WindAppCard.qml
        ui/pages/climate/SunAppCard.qml
        ui/pages/plants/PlantsRootPage.qml
        ui/pages/settings/SettingsRootPage.qml
        ui/pages/settings/SettingsNetworkPage.qml
        ui/pages/settings/SettingsWeatherPage.qml
        ui/pages/settings/SettingsSensorsPage.qml
        ui/pages/settings/SettingsDisplayPage.qml
        ui/pages/settings/SettingsNotificationPage.qml
        ui/pages/settings/SettingsPlantPage.qml
        ui/pages/settings/SettingsSystemPage.qml
        ui/theme/Style.qml
        ui/theme/AppColors.qml
        ui/theme/AppLayout.qml
        ui/theme/AppTypography.qml
        ui/theme/AppSizes.qml
        ui/theme/AppRadius.qml
        ui/utils/WeatherIconMapper.js
        ui/utils/ClimateIconMapper.js
        ui/utils/AirQualityMapper.js

    RESOURCES
        assets/BrushedMetal.jpg
        assets/under-construction.svg
        assets/weather/clear-day.svg
        assets/weather/clear-night.svg
        assets/weather/cloudy.svg
        assets/weather/fog.svg
        assets/weather/not-available.svg
        assets/weather/overcast-day-rain.svg
        assets/weather/overcast-night-rain.svg
        assets/weather/overcast.svg
        assets/weather/partly-cloudy-day-rain.svg
        assets/weather/partly-cloudy-day.svg
        assets/weather/partly-cloudy-night-rain.svg
        assets/weather/partly-cloudy-night.svg
        assets/weather/rain.svg
        assets/weather/thunderstorms-day-rain.svg
        assets/weather/thunderstorms-night-rain.svg
        assets/weather/snow.svg
        assets/weather/drop-white-icon.svg
        assets/weather/wind-arrow-white.svg
        assets/weather/sunrise50x50.png
        assets/weather/sunset50x50.png
        assets/climate/humidity-white.svg
        assets/climate/propeller-white.svg
        assets/climate/temperature-high.svg
        assets/climate/temperature-low.svg
        assets/climate/temperature-medium.svg
        assets/climate/emoji-sad-white.svg
        assets/climate/emoji-smiley-white.svg
        assets/climate/window-open-white.svg
        assets/climate/pressure-gauge-white.svg
        assets/climate/greenhouse-white.svg
        assets/climate/room-white.svg
        assets/climate/weather-white.svg
        assets/tabbar/climate-icon-white.svg
        assets/tabbar/climate-icon-dark.svg
        assets/tabbar/home-icon-white.svg
        assets/tabbar/home-icon-dark.svg
        assets/tabbar/plant-icon-white.svg
        assets/tabbar/plant-icon-dark.svg
        assets/tabbar/setting-icon-white.svg
        assets/tabbar/setting-icon-dark.svg
        assets/warning-sign.svg
        assets/settings/wifi-icon-white.svg
        assets/settings/settings-icon-white.svg
        assets/settings/watering-icon-white.svg
        assets/settings/notification-icon-white.svg
        assets/settings/sensor-icon-white.svg
        assets/settings/display-icon-white.svg
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appHomeNexus PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appHomeNexus
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appHomeNexus
    PRIVATE
        Qt6::Quick
        HomeNexusWeather
        HomeNexusUtils
        HomeNexusAppService
)

include(GNUInstallDirs)

install(TARGETS appHomeNexus
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Copy homenexus.example.ini to the build directory
add_custom_command(
    TARGET appHomeNexus POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:appHomeNexus>/config"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/config/homenexus.example.ini"
            "$<TARGET_FILE_DIR:appHomeNexus>/config/homenexus.ini"
)

# Copy weather and forecast fallback json files to the build directory
add_custom_command(
    TARGET appHomeNexus POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:appHomeNexus>/data/weather"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/data/weather/weather_fallback.json"
            "$<TARGET_FILE_DIR:appHomeNexus>/data/weather/weather_fallback.json"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/data/weather/forecast_fallback.json"
            "$<TARGET_FILE_DIR:appHomeNexus>/data/weather/forecast_fallback.json"
)
