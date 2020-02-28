project(XamlWebViewMeta CXX)

file(GLOB WEBVIEW_META_SOURCE "src/*.cpp")

add_library(xaml_ui_webview_meta SHARED ${WEBVIEW_META_SOURCE})

target_include_directories(xaml_ui_webview_meta
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xaml_ui_webview_meta xaml_ui_webview)

set_target_properties(xaml_ui_webview_meta PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION}
)
