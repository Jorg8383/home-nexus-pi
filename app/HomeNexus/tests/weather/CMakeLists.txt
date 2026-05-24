#--------------------------------------------------------------
# Test for the OpenWeatherClient class
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

#--------------------------------------------------------------
# Test for the OpenWeatherParser class
qt_add_executable(OpenWeatherParserTest
    OpenWeatherParserTest.cpp
)

target_link_libraries(OpenWeatherParserTest
    PRIVATE
        Qt6::Test
        Qt6::Core
        HomeNexusWeather
)

add_test(
    NAME OpenWeatherParserTest
    COMMAND OpenWeatherParserTest
)

set_tests_properties(OpenWeatherParserTest PROPERTIES
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
