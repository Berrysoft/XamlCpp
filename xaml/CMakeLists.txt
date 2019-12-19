project(Xaml CXX)

include_directories(include)

file(GLOB XAML_HEADERS "include/xaml/*.hpp")
file(GLOB XAML_SOURCES "src/*.cpp")

find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

include_directories(../meta/include)

add_library(xaml ${XAML_SOURCES})
target_link_libraries(xaml ${LIBXML2_LIBRARIES})
target_link_libraries(xaml xaml_meta)

install(FILES ${XAML_HEADERS} DESTINATION include/xaml)
install(TARGETS xaml EXPORT xaml_targets LIBRARY DESTINATION lib)

file(GLOB TEST_SOURCE "test/*.cpp")
add_executable(xaml_test ${TEST_SOURCE})
target_link_libraries(xaml_test xaml)
