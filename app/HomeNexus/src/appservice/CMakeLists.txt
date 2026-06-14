add_library(HomeNexusAppService STATIC
    IAppConfig.hpp
    AppConfig.hpp
    AppConfig.cpp
)

target_link_libraries(HomeNexusAppService
    PUBLIC
        Qt6::Core
)

target_include_directories(HomeNexusAppService
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(HomeNexusAppService
    PUBLIC
        cxx_std_17
)
