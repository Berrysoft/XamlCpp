project(XamlGlobal CXX)

file(GLOB GLOBAL_HEADERS "include/xaml/*.hpp")
file(GLOB GLOBAL_SOURCE "src/*.cpp")

add_library(xaml_global ${GLOBAL_SOURCE})

target_compile_definitions(xaml_global PUBLIC ${XAML_BUILD_DEFINITIONS})
target_compile_options(xaml_global PUBLIC ${XAML_COMPILE_OPTIONS})

if(CMAKE_VERSION VERSION_LESS 3.14)
    target_link_libraries(xaml_global INTERFACE ${XAML_LINK_OPTIONS})
else()
    target_link_options(xaml_global PUBLIC ${XAML_LINK_OPTIONS})
endif()

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_global PRIVATE "-DXAML_GLOBAL_API=__XAML_EXPORT")
endif()

target_include_directories(xaml_global
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(FILES ${GLOBAL_HEADERS} DESTINATION include/xaml)
