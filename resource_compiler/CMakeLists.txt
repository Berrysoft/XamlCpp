project(XamlResourceCompiler CXX)

file(GLOB XAML_RC_SOURCE "src/*.cpp")
add_executable(xamlrc ${XAML_RC_SOURCE})

target_include_directories(xamlrc PUBLIC include)

target_link_libraries(xamlrc xaml_cmdline)

install(TARGETS xamlrc RUNTIME DESTINATION bin)
