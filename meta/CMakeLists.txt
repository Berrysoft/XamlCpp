project(XamlMeta CXX)

file(GLOB UTILITY_HEADERS "include/xaml/*.hpp")
file(GLOB META_HEADERS "include/xaml/meta/*.hpp")
file(GLOB META_SOURCES "src/*.cpp")

add_library(xaml_meta ${META_SOURCES})

target_compile_definitions(xaml_meta PUBLIC ${XAML_BUILD_DEFINITIONS})
target_compile_options(xaml_meta PUBLIC ${XAML_COMPILE_OPTIONS})

if(CMAKE_VERSION VERSION_LESS 3.14)
    target_link_libraries(xaml_meta INTERFACE ${XAML_LINK_OPTIONS})
else()
    target_link_options(xaml_meta PUBLIC ${XAML_LINK_OPTIONS})
endif()

target_include_directories(xaml_meta
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(FILES ${UTILITY_HEADERS} DESTINATION include/xaml)
install(FILES ${META_HEADERS} DESTINATION include/xaml/meta)
install(TARGETS xaml_meta EXPORT xaml_meta-targets ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(EXPORT xaml_meta-targets DESTINATION lib/cmake/xaml_meta)

configure_file(cmake/xaml_meta-config.cmake.in ${PROJECT_BINARY_DIR}/xaml_meta-config.cmake @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/xaml_meta-config.cmake DESTINATION lib/cmake/xaml_meta)

add_subdirectory(test)
