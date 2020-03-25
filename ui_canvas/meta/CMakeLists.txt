project(XamlCanvasMeta CXX)

file(GLOB CV_META_SOURCE "src/*.cpp")

add_library(xaml_ui_canvas_meta SHARED ${CV_META_SOURCE})

target_compile_definitions(xaml_ui_canvas_meta PRIVATE "-DXAML_UI_CANVAS_META_API=__XAML_META_EXPORT")

target_link_libraries(xaml_ui_canvas_meta xaml_ui_canvas)
