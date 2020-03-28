project(XamlWebViewMeta CXX)

file(GLOB WEBVIEW_META_SOURCE "src/*.cpp")

add_library(xaml_ui_webview_meta SHARED ${WEBVIEW_META_SOURCE})

target_compile_definitions(xaml_ui_webview_meta PRIVATE "XAML_UI_WEBVIEW_META_API=__XAML_META_EXPORT")

target_link_libraries(xaml_ui_webview_meta xaml_ui_webview)
