project(GlobalTestC C)

file(GLOB TEST_SOURCE "src/*.c*")

add_executable(global_test_c ${TEST_SOURCE})
target_include_directories(global_test_c
    PUBLIC 
        include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(global_test_c xaml_global)
