project(XamlC CXX)

file(GLOB XAMLC_HEADERS "include/xaml/*.hpp")
file(GLOB XAMLC_SOURCES "src/*.cpp")

add_library(xamlc ${XAMLC_SOURCES})
target_link_libraries(xamlc xaml)

target_include_directories(xamlc
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(xamlc PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION}
)

install(FILES ${XAMLC_HEADERS} DESTINATION include/xaml)

add_subdirectory(test)
