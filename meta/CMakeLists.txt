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

if(UNIX)
    target_link_libraries(xaml_meta PUBLIC dl)
endif()

target_include_directories(xaml_meta
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(xaml_meta PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION}
)

install(FILES ${UTILITY_HEADERS} DESTINATION include/xaml)
install(FILES ${META_HEADERS} DESTINATION include/xaml/meta)

add_subdirectory(test)
