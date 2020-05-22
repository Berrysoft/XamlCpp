project(GlobalTestCXX CXX)

file(GLOB TEST_SOURCE "src/*.cpp")

add_executable(global_test_cxx ${TEST_SOURCE})
target_link_libraries(global_test_cxx xaml_global stream_format Boost::nowide)
