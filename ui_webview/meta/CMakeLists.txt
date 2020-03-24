project(XamlWebViewMeta CXX)

file(GLOB WEBVIEW_META_SOURCE "src/*.cpp")

add_library(xaml_ui_webview_meta MODULE ${WEBVIEW_META_SOURCE})

target_link_libraries(xaml_ui_webview_meta xaml_ui_webview)
