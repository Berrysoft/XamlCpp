project(XamlCompilerCpp CXX)

file(GLOB XAML_CPP_SOURCE "src/*.cpp")

add_executable(xamlcpp ${XAML_CPP_SOURCE})
target_link_libraries(xamlcpp xamlc)

target_link_libraries(xamlcpp Boost::program_options)

if(APPLE)
    target_link_libraries(xamlcpp Boost::filesystem)
endif()

install(TARGETS xamlcpp RUNTIME DESTINATION bin)
