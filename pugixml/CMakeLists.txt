project(pugixml VERSION 1.10 LANGUAGES CXX)

add_library(xaml_pugixml INTERFACE)
add_library(pugixml ALIAS xaml_pugixml)

target_include_directories(xaml_pugixml
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>)
