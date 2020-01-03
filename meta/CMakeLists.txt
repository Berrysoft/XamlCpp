project(XamlMeta CXX)

include_directories(include)

file(GLOB UTILITY_HEADERS "include/xaml/*.hpp")
file(GLOB META_HEADERS "include/xaml/meta/*.hpp")
file(GLOB META_SOURCES "src/*.cpp")

add_library(xaml_meta ${META_SOURCES})

install(FILES ${UTILITY_HEADERS} DESTINATION include/xaml)
install(FILES ${META_HEADERS} DESTINATION include/xaml/meta)
install(TARGETS xaml_meta EXPORT xaml_meta-targets LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(EXPORT xaml_meta-targets DESTINATION lib/cmake/xaml_meta)

configure_file(cmake/xaml_meta-config.cmake.in ${PROJECT_BINARY_DIR}/xaml_meta-config.cmake @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/xaml_meta-config.cmake DESTINATION lib/cmake/xaml_meta)

file(GLOB TEST_SOURCE "test/*.cpp")
add_executable(meta_test ${TEST_SOURCE})
target_link_libraries(meta_test xaml_meta)
