project(XamlTest CXX)

file(GLOB TEST_SOURCE "src/*.cpp")
add_executable(xaml_test ${TEST_SOURCE})

if(WIN32)
    set_target_properties(xaml_test PROPERTIES WIN32_EXECUTABLE ON)
endif()

set(XAMLRC_PATH ${XAML_RUNTIME_OUTPUT_DIRECTORY}/xamlrc CACHE STRING "Path of xamlrc executable")

set(XAMLRC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/parser/test)

include(XamlResourceHelper)

target_add_rc(xaml_test
    FILES view/test.xaml
    DEPENDS xaml_ui_controls xaml_ui_canvas xaml_resource xamlrc
    DESTINATION ${XAMLRC_OUTPUT_DIR}/rc.g.cpp
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(xaml_test xaml_ui_controls xaml_ui_canvas xaml_parser xaml_ui_appmain xaml_resource)
if(${BUILD_WINDOWS})
    target_link_libraries(xaml_test wil)
elseif(${BUILD_GTK3})
    target_link_libraries(xaml_test gtk3)
endif()

target_include_directories(xaml_test PUBLIC include)
