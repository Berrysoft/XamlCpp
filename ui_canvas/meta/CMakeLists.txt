project(XamlCanvasMeta CXX)

file(GLOB CV_META_SOURCE "src/*.cpp")

add_library(xaml_ui_canvas_meta SHARED ${CV_META_SOURCE})

target_include_directories(xaml_ui_canvas_meta
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xaml_ui_canvas_meta xaml_ui_canvas)
