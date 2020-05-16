project(XamlResourceCompiler CXX)

file(GLOB XAML_RC_SOURCE "src/*.cpp")
add_executable(xamlrc ${XAML_RC_SOURCE})

target_include_directories(xamlrc PUBLIC include)

target_link_libraries(xamlrc PUBLIC xaml_cmdline PRIVATE xaml_cmdline_helper)

if(${Boost_nowide_FOUND})
    target_link_libraries(xamlrc PUBLIC Boost::nowide)
endif()

install(TARGETS xamlrc RUNTIME DESTINATION bin)
