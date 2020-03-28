project(XamlCmdLine CXX)

file(GLOB CMDLINE_HEADERS "include/xaml/cmdline/*.hpp")
file(GLOB CMDLINE_SOURCES "src/*.cpp")

add_library(xaml_cmdline ${CMDLINE_SOURCES})
target_link_libraries(xaml_cmdline PUBLIC xaml_meta)

target_include_directories(xaml_cmdline
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_cmdline PRIVATE "XAML_CMDLINE_API=__XAML_EXPORT")
endif()

install(FILES ${CMDLINE_HEADERS} DESTINATION include/xaml/cmdline)
