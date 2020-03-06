project(XamlTest CXX)

file(GLOB TEST_SOURCE "src/*.cpp")
if(WIN32)
    add_executable(xaml_test WIN32 ${TEST_SOURCE})
    add_executable(xaml_test_fake WIN32 ${TEST_SOURCE})
else()
    add_executable(xaml_test ${TEST_SOURCE})
    add_executable(xaml_test_fake ${TEST_SOURCE})
endif()
target_link_libraries(xaml_test xaml_ui_controls xaml_ui_canvas)
target_link_libraries(xaml_test_fake xaml_ui_controls xaml_ui_canvas xaml)

target_compile_definitions(xaml_test_fake PRIVATE "-DXAML_TEST_GEN_FAKE")

configure_file(view/test.xaml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.xaml COPYONLY)

add_custom_target(xaml_test_gen DEPENDS xaml_ui_meta xaml_ui_controls_meta xaml_ui_canvas_meta xamlcpp)
add_custom_command(TARGET xaml_test_gen POST_BUILD
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/xamlcpp test.xaml -o test.xaml.g.cpp --no-logo -L ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} -L ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
add_custom_target(xaml_test_gen_fake DEPENDS xaml_ui_meta xaml_ui_controls_meta xaml_ui_canvas_meta xamlcpp)
add_custom_command(TARGET xaml_test_gen_fake POST_BUILD
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/xamlcpp test.xaml -o test.xaml.g.fake.cpp --no-logo -f -L ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} -L ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_dependencies(xaml_test xaml_test_gen)
add_dependencies(xaml_test_fake xaml_test_gen_fake)

target_include_directories(xaml_test
    PUBLIC include
    PRIVATE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
target_include_directories(xaml_test_fake
    PUBLIC include
    PRIVATE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
