add_library(HomeNexusWeather STATIC
    OpenWeatherClient.hpp
    OpenWeatherClient.cpp
    OpenWeatherParser.hpp
    OpenWeatherParser.cpp
    WeatherData.hpp
    ForecastEntry.hpp
    ForecastData.hpp
    OpenWeatherClient.hpp
    OpenWeatherClient.cpp
    GeoCodingClient.hpp
    GeoCodingClient.cpp
    GeoLocation.hpp
    WeatherService.hpp
    WeatherService.cpp
    WeatherRepository.hpp
    WeatherRepository.cpp
    ForecastListModel.hpp
    ForecastListModel.cpp
    WeatherViewModel.hpp
    WeatherViewModel.cpp
    WeatherFallbackProvider.hpp
    WeatherFallbackProvider.cpp
    WeatherNotificationTypes.hpp
)

target_link_libraries(HomeNexusWeather
    PUBLIC
        Qt6::Core
        HomeNexusAppService
    PRIVATE
        Qt6::Network
        HomeNexusUtils
)

target_include_directories(HomeNexusWeather
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(HomeNexusWeather
    PUBLIC
        cxx_std_17
)
