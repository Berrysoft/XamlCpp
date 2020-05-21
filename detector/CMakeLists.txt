project(XamlDetector CXX)

file(GLOB XAMLD_SOURCE "src/*.cpp")

add_executable(xamld ${XAMLD_SOURCE})
target_include_directories(xamld PUBLIC include)
target_link_libraries(xamld PUBLIC xaml_cmdline PRIVATE xaml_cmdline_helper stream_format)

install(TARGETS xamld RUNTIME DESTINATION bin)
