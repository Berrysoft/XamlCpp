project(XamlCompilerCpp CXX)

file(GLOB XAML_CPP_SOURCE "src/*.cpp")

add_executable(xamlcpp ${XAML_CPP_SOURCE})
target_link_libraries(xamlcpp PUBLIC xaml_compiler xaml_cmdline)

install(TARGETS xamlcpp RUNTIME DESTINATION bin)
