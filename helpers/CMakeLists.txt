project(XamlHelpers CXX)

add_library(xaml_helpers INTERFACE)

target_include_directories(xaml_helpers INTERFACE include)

if(APPLE)
    target_link_libraries(xaml_helpers INTERFACE Boost::iostreams)
endif()
