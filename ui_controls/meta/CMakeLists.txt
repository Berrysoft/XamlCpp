project(XamlUIControlsMeta CXX)

file(GLOB UI_CONTROLS_META_SOURCE "src/*.cpp")

add_library(xaml_ui_controls_meta MODULE ${UI_CONTROLS_META_SOURCE})

target_link_libraries(xaml_ui_controls_meta xaml_ui_controls)
