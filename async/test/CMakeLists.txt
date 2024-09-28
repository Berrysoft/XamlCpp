project(AsyncTest CXX)

file(GLOB TEST_SOURCE "src/*.cpp")

add_executable(async_test ${TEST_SOURCE})
target_link_libraries(async_test xaml_async stream_format nowide)
