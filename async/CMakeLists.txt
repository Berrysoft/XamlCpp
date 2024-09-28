project(XamlAsync CXX)

file(GLOB ASYNC_HEADERS "include/xaml/async/*.h")
file(GLOB ASYNC_SOURCE "src/*.cpp")
add_library(xaml_async ${ASYNC_SOURCE})

target_include_directories(xaml_async
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(xaml_async PUBLIC xaml_global xaml_meta)

if(${BUILD_SHARED_LIBS})
	target_compile_definitions(xaml_async PRIVATE "XAML_ASYNC_API=__XAML_EXPORT")
endif()

if(${XAML_INSTALL})
	install(FILES ${ASYNC_HEADERS} DESTINATION include/xaml)
endif()

if(${BUILD_TESTS})
    add_subdirectory(test)
endif()
