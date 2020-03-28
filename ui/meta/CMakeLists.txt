project(XamlUIMeta CXX)

file(GLOB UI_META_SOURCE "src/*.cpp")

add_library(xaml_ui_meta SHARED ${UI_META_SOURCE})

target_compile_definitions(xaml_ui_meta PRIVATE "XAML_UI_META_API=__XAML_META_EXPORT")

target_link_libraries(xaml_ui_meta xaml_ui)
