project(GlobalTestC C)

file(GLOB TEST_SOURCE "src/*.c")

add_executable(global_test_c ${TEST_SOURCE})
target_link_libraries(global_test_c xaml_global)
