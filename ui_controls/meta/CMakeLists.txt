project(XamlUIControlsMeta CXX)

file(GLOB UI_CONTROLS_META_HEADERS "include/xaml/ui/controls/*.hpp")
file(GLOB UI_CONTROLS_META_SOURCE "src/*.cpp")

add_library(xaml_ui_controls_meta ${UI_CONTROLS_META_SOURCE})

target_include_directories(xaml_ui_controls_meta
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xaml_ui_controls_meta xaml_ui_controls)

set_target_properties(xaml_ui_controls_meta PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION}
)

install(FILES ${UI_CONTROLS_META_HEADERS} DESTINATION include/xaml/ui/controls)
install(TARGETS xaml_ui_controls_meta EXPORT xaml_ui_controls_meta-targets ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(EXPORT xaml_ui_controls_meta-targets DESTINATION lib/cmake/xaml_ui_controls_meta)

configure_file(cmake/xaml_ui_controls_meta-config.cmake.in ${PROJECT_BINARY_DIR}/xaml_ui_controls_meta-config.cmake @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/xaml_ui_controls_meta-config.cmake DESTINATION lib/cmake/xaml_ui_controls_meta)
