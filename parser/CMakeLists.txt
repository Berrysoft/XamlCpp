project(XamlParser CXX)

file(GLOB XAML_HEADERS "include/xaml/*.hpp")
file(GLOB XAML_MARKUP_HEADERS "include/xaml/markup/*.hpp")
file(GLOB XAML_SOURCES "src/*.cpp")

add_library(xaml_parser ${XAML_SOURCES})
target_link_libraries(xaml_parser PUBLIC xaml_meta PRIVATE pugixml)

target_include_directories(xaml_parser
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_parser PRIVATE "-DXAML_API=__XAML_EXPORT")
endif()

install(FILES ${XAML_HEADERS} DESTINATION include/xaml)
install(FILES ${XAML_MARKUP_HEADERS} DESTINATION include/xaml/markup)
