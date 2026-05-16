cmake_minimum_required(VERSION 3.16)

project(HomeNexus VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appHomeNexus
    main.cpp
)

set_source_files_properties(theme/Style.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

set_source_files_properties(utils/WeatherIconMapper.js PROPERTIES
    QT_QML_SKIP_QMLDIR_ENTRY TRUE
)

set_source_files_properties(utils/ClimateIconMapper.js PROPERTIES
    QT_QML_SKIP_QMLDIR_ENTRY TRUE
)

qt_add_qml_module(appHomeNexus
    URI HomeNexus
    VERSION 1.0
    QML_FILES
        Main.qml
        components/AppHeader.qml
        components/AppTabButton.qml
        components/AppButton.qml
        components/AppCard.qml
        components/ClimateMetricCard.qml
        components/ClimateSensorOverview.qml
        components/AppTopicCard.qml
        data/StaticForecastModel.qml
        data/StaticWeatherModel.qml
        pages/home/HomeRootPage.qml
        pages/climate/ClimateGreenhousePage.qml
        pages/climate/ClimateRootPage.qml
        pages/climate/ClimateIndoorPage.qml
        pages/climate/ClimateWeatherPage.qml
        pages/climate/ForecastDelegate.qml
        pages/climate/WeatherAppCard.qml
        pages/climate/WindAppCard.qml
        pages/climate/SunAppCard.qml
        pages/plants/PlantsRootPage.qml
        pages/settings/SettingsRootPage.qml
        theme/Style.qml
        theme/AppColors.qml
        theme/AppLayout.qml
        theme/AppTypography.qml
        theme/AppSizes.qml
        theme/AppRadius.qml
        utils/WeatherIconMapper.js
        utils/ClimateIconMapper.js

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
        assets/climate/thumbs-down-orange.svg
        assets/climate/thumbs-down-red.svg
        assets/climate/thumbs-up-green.svg
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
)

include(GNUInstallDirs)
install(TARGETS appHomeNexus
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
