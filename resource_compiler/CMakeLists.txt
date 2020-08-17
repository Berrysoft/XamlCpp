project(XamlResourceCompiler CXX)

file(GLOB XAML_RC_SOURCE "src/*.cpp")
add_executable(xamlrc ${XAML_RC_SOURCE})

target_include_directories(xamlrc
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(xamlrc PUBLIC xaml_cmdline PRIVATE xaml_cmdline_helper xaml_helpers stream_format nowide)
