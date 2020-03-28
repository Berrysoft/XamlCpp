project(XamlUIControlsMeta CXX)

file(GLOB UI_CONTROLS_META_SOURCE "src/*.cpp")

add_library(xaml_ui_controls_meta SHARED ${UI_CONTROLS_META_SOURCE})

target_compile_definitions(xaml_ui_controls_meta PRIVATE "XAML_UI_CONTROLS_META_API=__XAML_META_EXPORT")

target_link_libraries(xaml_ui_controls_meta xaml_ui_controls)
