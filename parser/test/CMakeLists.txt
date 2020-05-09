project(XamlTest CXX)

file(GLOB TEST_SOURCE "src/*.cpp")
add_executable(xaml_test ${TEST_SOURCE})

if(WIN32)
    set_target_properties(xaml_test PROPERTIES WIN32_EXECUTABLE ON)
endif()

target_link_libraries(xaml_test xaml_ui_controls xaml_ui_canvas xaml_parser xaml_ui_appmain)
if(${BUILD_WINDOWS})
    target_link_libraries(xaml_test wil)
elseif(${BUILD_GTK3})
    target_link_libraries(xaml_test gtk3)
endif()

target_include_directories(xaml_test PUBLIC include)

configure_file(view/test.xaml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.xaml COPYONLY)
