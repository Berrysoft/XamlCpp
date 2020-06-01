project(XamlMarkup CXX)

file(GLOB XAML_MARKUP_HEADERS "include/xaml/markup/*.h")
file(GLOB XAML_MARKUP_SOURCES "src/*.cpp")

add_library(xaml_markup ${XAML_MARKUP_SOURCES})
target_link_libraries(xaml_markup PUBLIC xaml_meta)

target_include_directories(xaml_markup
	PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_markup PRIVATE "XAML_MARKUP_API=__XAML_EXPORT")
endif()

install(FILES ${XAML_MARKUP_HEADERS} DESTINATION include/xaml/markup)
