project(XamlCompilerCpp CXX)

file(GLOB XAML_CPP_SOURCE "src/*.cpp")

add_executable(xamlcpp ${XAML_CPP_SOURCE})
target_link_libraries(xamlcpp xamlc)

find_package(Boost REQUIRED COMPONENTS program_options)

target_link_libraries(xamlcpp Boost::program_options)

install(TARGETS xamlcpp RUNTIME DESTINATION bin)
