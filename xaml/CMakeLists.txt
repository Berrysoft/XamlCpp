project(Xaml CXX)

file(GLOB XAML_HEADERS "include/xaml/*.hpp")
file(GLOB XAML_MARKUP_HEADERS "include/xaml/markup/*.hpp")
file(GLOB XAML_SOURCES "src/*.cpp")

add_library(xaml ${XAML_SOURCES})
target_link_libraries(xaml PUBLIC xaml_meta)

find_package(pugixml REQUIRED)
target_link_libraries(xaml PRIVATE pugixml)

target_include_directories(xaml
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(FILES ${XAML_HEADERS} DESTINATION include/xaml)
install(FILES ${XAML_MARKUP_HEADERS} DESTINATION include/xaml/markup)
