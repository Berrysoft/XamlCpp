project(XamlHelpers CXX)

add_library(xaml_helpers INTERFACE)

target_include_directories(xaml_helpers INTERFACE include)

target_link_libraries(xaml_helpers INTERFACE xaml_global)

if(WIN32)
    target_link_libraries(xaml_helpers INTERFACE nowide)
endif()
