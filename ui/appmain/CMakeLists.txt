project(XamlUIAppMain CXX)

file(GLOB APPMAIN_SOURCE "src/*.cpp")

add_library(xaml_ui_appmain OBJECT ${APPMAIN_SOURCE})

target_include_directories(xaml_ui
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(xaml_ui_appmain xaml_ui)
