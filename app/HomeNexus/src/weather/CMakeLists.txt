add_library(HomeNexusWeather STATIC
    OpenWeatherClient.hpp
    OpenWeatherClient.cpp
    OpenWeatherParser.hpp
    OpenWeatherParser.cpp
    CurrentWeather.hpp
    ForecastEntry.hpp
    WeatherForecast.hpp
    OpenWeatherClient.hpp
    OpenWeatherClient.cpp
    GeoCodingClient.hpp
    GeoCodingClient.cpp
)

target_link_libraries(HomeNexusWeather
    PUBLIC
        Qt6::Core
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
