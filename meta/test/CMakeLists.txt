project(XamlMetaTest CXX)

file(GLOB TEST_SOURCE "src/*.cpp")
add_executable(meta_test ${TEST_SOURCE})
target_link_libraries(meta_test xaml_meta)
