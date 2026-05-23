qt_add_executable(OpenWeatherClientTest
    OpenWeatherClientTest.cpp
)

target_link_libraries(OpenWeatherClientTest
    PRIVATE
        Qt6::Test
        Qt6::Network
        HomeNexusWeather
)

add_test(
    NAME OpenWeatherClientTest
    COMMAND OpenWeatherClientTest
)
