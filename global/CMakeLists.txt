project(XamlGlobal CXX)

file(GLOB GLOBAL_HEADERS "include/xaml/*.hpp")

add_library(xaml_global INTERFACE)

target_compile_definitions(xaml_global
    INTERFACE
        ${XAML_BUILD_DEFINITIONS}
        $<BUILD_INTERFACE:${XAML_BUILD_INTERNAL_DEFINITIONS}>
)

target_compile_options(xaml_global INTERFACE ${XAML_COMPILE_OPTIONS})

target_link_options(xaml_global INTERFACE ${XAML_LINK_OPTIONS})

target_include_directories(xaml_global
    INTERFACE 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

install(FILES ${GLOBAL_HEADERS} DESTINATION include/xaml)
