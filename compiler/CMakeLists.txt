project(XamlC CXX)

file(GLOB XAMLC_HEADERS "include/xaml/*.hpp")
file(GLOB XAMLC_SOURCES "src/*.cpp")

add_library(xaml_compiler ${XAMLC_SOURCES})
target_link_libraries(xaml_compiler PUBLIC xaml_parser)

target_include_directories(xaml_compiler
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_compiler PRIVATE "XAML_COMPILER_API=__XAML_EXPORT")
endif()

install(FILES ${XAMLC_HEADERS} DESTINATION include/xaml)

if(${BUILD_TESTS})
    add_subdirectory(test)
endif()
