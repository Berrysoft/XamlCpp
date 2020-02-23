project(XamlWebViewMeta CXX)

file(GLOB WEBVIEW_META_HEADERS "include/xaml/ui/webview/*.hpp")
file(GLOB WEBVIEW_META_SOURCE "src/*.cpp")

add_library(xaml_ui_webview_meta SHARED ${WEBVIEW_META_SOURCE})

target_include_directories(xaml_ui_webview_meta
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xaml_ui_webview_meta xaml_ui_webview)

set_target_properties(xaml_ui_webview_meta PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION}
)

install(FILES ${WEBVIEW_META_HEADERS} DESTINATION include/xaml/ui/webview)
install(TARGETS xaml_ui_webview_meta EXPORT xaml_ui_webview_meta-targets ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(EXPORT xaml_ui_webview_meta-targets DESTINATION lib/cmake/xaml_ui_webview_meta)
