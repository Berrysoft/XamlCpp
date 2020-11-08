project(XamlGlobal CXX)

file(GLOB GLOBAL_HEADERS "include/xaml/*.h*")
file(GLOB GLOBAL_SOURCE "src/*.cpp")
add_library(xaml_global ${GLOBAL_SOURCE})

target_compile_definitions(xaml_global
    PUBLIC
        ${XAML_BUILD_DEFINITIONS}
        $<BUILD_INTERFACE:${XAML_BUILD_INTERNAL_DEFINITIONS}>
)

target_compile_options(xaml_global
    PUBLIC
        ${XAML_COMPILE_OPTIONS}
        $<$<COMPILE_LANGUAGE:C>:${XAML_C_COMPILE_OPTIONS}>
        $<$<COMPILE_LANGUAGE:CXX>:${XAML_CXX_COMPILE_OPTIONS}>
)

if(${BUILD_COCOA})
    target_compile_options(xaml_global
        PUBLIC
            $<$<COMPILE_LANGUAGE:OBJC,OBJCXX>:${XAML_OBJC_COMPILE_OPTIONS}>
            $<$<COMPILE_LANGUAGE:OBJCXX>:${XAML_CXX_COMPILE_OPTIONS}>
    )
endif()

target_link_options(xaml_global PUBLIC ${XAML_LINK_OPTIONS})

target_include_directories(xaml_global
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(xaml_global PRIVATE stream_format nowide PUBLIC function2)

target_compile_definitions(xaml_global
    PUBLIC
        $<BUILD_INTERFACE:"XAML_BUILD">
)

target_compile_definitions(xaml_global PRIVATE "XAML_GLOBAL_BUILD")

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_global PRIVATE "XAML_API=__XAML_EXPORT")
endif()

if(${XAML_INSTALL})
    install(FILES ${GLOBAL_HEADERS} DESTINATION include/xaml)
endif()

if(${BUILD_TESTS})
    add_subdirectory(test)
endif()
