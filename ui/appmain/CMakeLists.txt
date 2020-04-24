project(XamlUIAppMain CXX)

file(GLOB APPMAIN_SOURCE "src/*.cpp")

add_library(xaml_ui_appmain STATIC ${APPMAIN_SOURCE})

target_link_libraries(xaml_ui_appmain xaml_ui)
