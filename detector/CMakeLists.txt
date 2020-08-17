project(XamlDetector CXX)

file(GLOB XAMLD_SOURCE "src/*.cpp")

add_executable(xamld ${XAMLD_SOURCE})
target_include_directories(xamld
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(xamld PUBLIC xaml_cmdline PRIVATE xaml_cmdline_helper stream_format nowide)
