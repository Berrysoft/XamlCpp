project(XamlUIMeta CXX)

file(GLOB UI_META_HEADERS "include/xaml/ui/*.hpp")
file(GLOB UI_META_SOURCE "src/*.cpp")

add_library(xaml_ui_meta ${UI_META_SOURCE})

target_include_directories(xaml_ui_meta
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xaml_ui_meta xaml_ui)

install(FILES ${UI_META_HEADERS} DESTINATION include/xaml/ui)
install(TARGETS xaml_ui_meta EXPORT xaml_ui_meta-targets ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(EXPORT xaml_ui_meta-targets DESTINATION lib/cmake/xaml_ui_meta)
