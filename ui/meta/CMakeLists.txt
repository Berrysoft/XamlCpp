project(XamlUIMeta CXX)

file(GLOB UI_META_HEADERS "include/xaml/ui/*.hpp")
file(GLOB UI_META_SOURCE "src/*.cpp")

add_library(xaml_ui_meta SHARED ${UI_META_SOURCE})

target_include_directories(xaml_ui_meta
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xaml_ui_meta xaml_ui)

set_target_properties(xaml_ui_meta PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION}
)

install(FILES ${UI_META_HEADERS} DESTINATION include/xaml/ui)
