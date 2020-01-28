project(XamlCompilerTest CXX)

file(GLOB TEST_SOURCE "src/*.cpp")
add_executable(xamlc_test ${TEST_SOURCE})
target_include_directories(xamlc_test PUBLIC include)
target_link_libraries(xamlc_test xamlc)
