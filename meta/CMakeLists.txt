project(XamlMeta CXX)

include_directories(include)

file(GLOB META_HEADERS "include/xaml/meta/*.hpp")
file(GLOB META_SOURCES "src/*.cpp")

add_library(xaml_meta ${META_SOURCES})

install(FILES ${META_HEADERS} DESTINATION include/xaml/meta)
install(TARGETS xaml_meta EXPORT xaml_meta_targets LIBRARY DESTINATION lib)

file(GLOB TEST_SOURCE "test/*.cpp")
add_executable(meta_test ${TEST_SOURCE})
target_link_libraries(meta_test xaml_meta)

