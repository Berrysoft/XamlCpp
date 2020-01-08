project(XamlC CXX)

file(GLOB XAMLC_HEADERS "include/xaml/*.hpp")
file(GLOB XAMLC_SOURCES "src/*.cpp")

add_library(xamlc ${XAMLC_SOURCES})
target_link_libraries(xamlc xaml)

target_include_directories(xamlc
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(${USE_PCH})
    target_precompile_headers(xamlc REUSE_FROM xaml_ui)
endif()

install(FILES ${XAMLC_HEADERS} DESTINATION include/xaml)
install(TARGETS xamlc EXPORT xamlc-targets ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(EXPORT xamlc-targets DESTINATION lib/cmake/xamlc)

configure_file(cmake/xamlc-config.cmake.in ${PROJECT_BINARY_DIR}/xamlc-config.cmake @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/xamlc-config.cmake DESTINATION lib/cmake/xamlc)

add_subdirectory(test)
