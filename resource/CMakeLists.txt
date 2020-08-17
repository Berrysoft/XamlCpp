project(XamlResource)

file(GLOB XAML_RESOURCE_HEADERS "include/xaml/resource/*.h")

add_library(xaml_resource INTERFACE)

target_include_directories(xaml_resource
    INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(xaml_resource INTERFACE xaml_global)

if(${XAML_INSTALL})
    install(FILES ${XAML_RESOURCE_HEADERS} DESTINATION include/xaml/resource)
endif()