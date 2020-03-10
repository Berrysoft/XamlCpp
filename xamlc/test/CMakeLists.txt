project(XamlTest CXX)

configure_file(view/test.xaml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.xaml COPYONLY)

add_custom_command(
    OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.xaml.g.cpp
    DEPENDS xaml_ui_meta xaml_ui_controls_meta xaml_ui_canvas_meta xamlcpp
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/xamlcpp test.xaml -o test.xaml.g.cpp --no-logo -L ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} -L ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_custom_command(
    OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.xaml.g.fake.cpp
    DEPENDS xaml_ui_meta xaml_ui_controls_meta xaml_ui_canvas_meta xamlcpp
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/xamlcpp test.xaml -o test.xaml.g.fake.cpp --no-logo -f -L ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} -L ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

file(GLOB TEST_SOURCE "src/*.cpp")
add_executable(xaml_test ${TEST_SOURCE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.xaml.g.cpp)
add_executable(xaml_test_fake ${TEST_SOURCE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.xaml.g.fake.cpp)

if(WIN32)
    set_target_properties(xaml_test xaml_test_fake PROPERTIES WIN32_EXECUTABLE ON)
endif()

target_link_libraries(xaml_test xaml_ui_controls xaml_ui_canvas)
target_link_libraries(xaml_test_fake xaml_ui_controls xaml_ui_canvas xaml)

target_compile_definitions(xaml_test_fake PRIVATE "-DXAML_TEST_GEN_FAKE")

target_include_directories(xaml_test
    PUBLIC include
    PRIVATE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
target_include_directories(xaml_test_fake
    PUBLIC include
    PRIVATE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
