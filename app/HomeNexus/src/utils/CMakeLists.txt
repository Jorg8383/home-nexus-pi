add_library(HomeNexusUtils STATIC
    json/JsonReader.hpp
    json/JsonReader.cpp
    IOpenWeatherConfig.hpp
    AppConfig.hpp
    AppConfig.cpp
)

target_link_libraries(HomeNexusUtils
    PUBLIC
        Qt6::Core
)

target_include_directories(HomeNexusUtils
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(HomeNexusUtils
    PUBLIC
        cxx_std_17
)
