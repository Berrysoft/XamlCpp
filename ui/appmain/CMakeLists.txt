project(XamlUIAppMain CXX)

file(GLOB APPMAIN_SOURCE "src/*.cpp")

add_library(xaml_ui_appmain OBJECT ${APPMAIN_SOURCE})

target_link_libraries(xaml_ui_appmain PUBLIC xaml_ui PRIVATE stream_format nowide)

if(WIN32 AND MSVC)
	target_link_options(xaml_ui_appmain PUBLIC "/entry:mainCRTStartup")
endif()

if(${BUILD_WINRT})
	target_link_libraries(xaml_ui_appmain PUBLIC runtimeobject)
endif()
