project(XamlTest CXX)

file(GLOB TEST_SOURCE "src/*.cpp")
if(WIN32)
    add_executable(xaml_test WIN32 ${TEST_SOURCE})
else()
    add_executable(xaml_test ${TEST_SOURCE})
endif()
target_include_directories(xaml_test PUBLIC include)
target_link_libraries(xaml_test xaml)

configure_file(view/test.xaml ${CMAKE_BINARY_DIR}/bin/test.xaml COPYONLY)
