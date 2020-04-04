project(XamlTest CXX)

file(GLOB TEST_SOURCE "src/*.cpp")
add_executable(xaml_test ${TEST_SOURCE})
add_executable(xaml_test_fake ${TEST_SOURCE})

set(XAMLCPP_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/xamlcpp)

include(XamlViewHelper)

set(XAMLC_OUTPUT_DIR ${CMAKE_BINARY_DIR}/compiler/test)

target_add_view(xaml_test
    DESTINATION ${XAMLC_OUTPUT_DIR}
    VIEWS ${PROJECT_SOURCE_DIR}/view/test.xaml
    DEPENDS xaml_ui_meta xaml_ui_controls_meta xaml_ui_canvas_meta xamlcpp
    SEARCH_PATHS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

target_add_view(xaml_test_fake FAKE
    DESTINATION ${XAMLC_OUTPUT_DIR}
    VIEWS ${PROJECT_SOURCE_DIR}/view/test.xaml
    DEPENDS xaml_ui_meta xaml_ui_controls_meta xaml_ui_canvas_meta xamlcpp
    SEARCH_PATHS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

if(WIN32)
    set_target_properties(xaml_test xaml_test_fake PROPERTIES WIN32_EXECUTABLE ON)
endif()

target_link_libraries(xaml_test xaml_ui_controls xaml_ui_canvas)
target_link_libraries(xaml_test_fake xaml_ui_controls xaml_ui_canvas xaml_parser)

target_compile_definitions(xaml_test_fake PRIVATE "XAML_TEST_GEN_FAKE")

target_include_directories(xaml_test PUBLIC include)
target_include_directories(xaml_test_fake PUBLIC include)
