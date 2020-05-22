project(XamlGlobal CXX)

file(GLOB GLOBAL_HEADERS "include/xaml/*.h*")
file(GLOB GLOBAL_SOURCE "src/*.cpp")
add_library(xaml_global ${GLOBAL_SOURCE})

target_compile_definitions(xaml_global
    PUBLIC
        ${XAML_BUILD_DEFINITIONS}
        $<BUILD_INTERFACE:${XAML_BUILD_INTERNAL_DEFINITIONS}>
)

target_compile_options(xaml_global PUBLIC ${XAML_COMPILE_OPTIONS})

target_link_options(xaml_global PUBLIC ${XAML_LINK_OPTIONS})

target_include_directories(xaml_global
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(xaml_global PRIVATE stream_format)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_global PRIVATE "XAML_API=__XAML_EXPORT")
endif()

install(FILES ${GLOBAL_HEADERS} DESTINATION include/xaml)

if(${BUILD_TESTS})
    add_subdirectory(test)
endif()
