project(Xaml CXX)

file(GLOB XAML_HEADERS "include/xaml/*.hpp")
file(GLOB XAML_MARKUP_HEADERS "include/xaml/markup/*.hpp")
file(GLOB XAML_SOURCES "src/*.cpp")

find_package(LibXml2 REQUIRED)

add_library(xaml ${XAML_SOURCES})
target_link_libraries(xaml ${LIBXML2_LIBRARIES})
target_link_libraries(xaml xaml_ui)

target_include_directories(xaml
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${LIBXML2_INCLUDE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(FILES ${XAML_HEADERS} DESTINATION include/xaml)
install(FILES ${XAML_MARKUP_HEADERS} DESTINATION include/xaml/markup)
install(TARGETS xaml EXPORT xaml-targets LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(EXPORT xaml-targets DESTINATION lib/cmake/xaml)

configure_file(cmake/xaml-config.cmake.in ${PROJECT_BINARY_DIR}/xaml-config.cmake @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/xaml-config.cmake DESTINATION lib/cmake/xaml)

add_subdirectory(test)