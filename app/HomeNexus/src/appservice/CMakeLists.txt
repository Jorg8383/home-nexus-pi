add_library(HomeNexusAppService STATIC
    IAppConfig.hpp
    AppConfig.hpp
    AppConfig.cpp
    AppNotificationTypes.hpp
    AppNotificationCenter.hpp
    AppNotificationCenter.cpp
    AppNotificationClient.hpp
    AppNotificationClient.cpp
    IAppNotificationClient.hpp
    NetworkStatus.hpp
    NetworkStatus.cpp
)

target_link_libraries(HomeNexusAppService
    PUBLIC
        Qt6::Core
    PRIVATE
        Qt6::Network
)

target_include_directories(HomeNexusAppService
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(HomeNexusAppService
    PUBLIC
        cxx_std_17
)
