project(Xaml CXX)

include_directories(include)
include_directories(../meta/include)
include_directories(../ui/include)
include_directories(${XAML_PRIVATE_INCLUDE_DIRS})

file(GLOB XAML_HEADERS "include/xaml/*.hpp")
file(GLOB XAML_SOURCES "src/*.cpp")

find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

add_library(xaml ${XAML_SOURCES})
target_link_libraries(xaml ${LIBXML2_LIBRARIES})
target_link_libraries(xaml xaml_ui)

if(NOT (CMAKE_VERSION VERSION_LESS 3.16))
    target_precompile_headers(xaml REUSE_FROM xaml_ui)
endif()

install(FILES ${XAML_HEADERS} DESTINATION include/xaml)
install(TARGETS xaml DESTINATION lib EXPORT xaml-targets)
install(EXPORT xaml-targets DESTINATION lib/cmake/xaml)

configure_file(cmake/xaml-config.cmake.in ${PROJECT_BINARY_DIR}/xaml-config.cmake @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/xaml-config.cmake DESTINATION lib/cmake/xaml)

file(GLOB TEST_SOURCE "test/*.cpp")
if(${BUILD_WINDOWS})
    add_executable(xaml_test WIN32 ${TEST_SOURCE})
else()
    add_executable(xaml_test ${TEST_SOURCE})
endif()
target_link_libraries(xaml_test xaml)

if(NOT (CMAKE_VERSION VERSION_LESS 3.16))
    target_precompile_headers(xaml_test REUSE_FROM xaml_ui)
endif()

configure_file(test/test.xaml test.xaml COPYONLY)
