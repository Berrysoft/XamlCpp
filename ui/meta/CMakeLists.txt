project(XamlUIMeta CXX)

file(GLOB UI_META_SOURCE "src/*.cpp")

add_library(xaml_ui_meta SHARED ${UI_META_SOURCE})

target_include_directories(xaml_ui_meta
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xaml_ui_meta xaml_ui)
