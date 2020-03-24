project(XamlUIMeta CXX)

file(GLOB UI_META_SOURCE "src/*.cpp")

add_library(xaml_ui_meta MODULE ${UI_META_SOURCE})

target_link_libraries(xaml_ui_meta xaml_ui)
