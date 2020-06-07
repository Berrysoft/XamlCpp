project(XamlMeta CXX)

file(GLOB META_HEADERS "include/xaml/meta/*.h*")
file(GLOB META_SOURCES "src/*.cpp")

add_library(xaml_meta ${META_SOURCES})

target_link_libraries(xaml_meta PUBLIC xaml_global PRIVATE nowide)

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

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_meta PRIVATE "XAML_META_API=__XAML_EXPORT")
endif()

install(FILES ${META_HEADERS} DESTINATION include/xaml/meta)

if(${BUILD_TESTS})
    add_subdirectory(test)
endif()
