project(XamlParser CXX)

file(GLOB XAML_HEADERS "include/xaml/parser/*.h")
file(GLOB XAML_SOURCES "src/*.cpp")

add_library(xaml_parser ${XAML_SOURCES})
target_link_libraries(xaml_parser PUBLIC xaml_meta xaml_markup PRIVATE xaml_rapidxml xaml_helpers stream_format)

target_include_directories(xaml_parser
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_parser PRIVATE "XAML_PARSER_API=__XAML_EXPORT")
endif()

if(${XAML_INSTALL})
    install(FILES ${XAML_HEADERS} DESTINATION include/xaml/parser)
endif()

if(${BUILD_TESTS})
    add_subdirectory(test)
endif()
