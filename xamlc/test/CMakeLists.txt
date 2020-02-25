project(XamlTest CXX)

file(GLOB TEST_SOURCE "src/*.cpp")
if(WIN32)
    add_executable(xaml_test WIN32 ${TEST_SOURCE})
else()
    add_executable(xaml_test ${TEST_SOURCE})
endif()
target_include_directories(xaml_test PUBLIC include)
target_link_libraries(xaml_test xaml_ui_controls)

configure_file(view/test.xaml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test.xaml COPYONLY)

option(XAML_TEST_GEN_FAKE "Generate fake init function to use parser." ON)

if(${XAML_TEST_GEN_FAKE})
    set(XAMLCPP_FAKE_COMMAND "-f")
    target_compile_definitions(xaml_test PRIVATE "-DXAML_TEST_GEN_FAKE")
    target_link_libraries(xaml_test xaml)
else()
    set(XAMLCPP_FAKE_COMMAND "")
endif()

add_custom_target(xaml_test_gen DEPENDS xaml_meta xaml_ui_controls_meta xamlcpp)
add_custom_command(TARGET xaml_test_gen POST_BUILD
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/xamlcpp test.xaml --no-logo ${XAMLCPP_FAKE_COMMAND} -L ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} -L ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_dependencies(xaml_test xaml_test_gen)
target_include_directories(xaml_test PRIVATE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
