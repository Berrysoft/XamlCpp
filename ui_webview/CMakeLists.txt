project(XamlWebView CXX)

if(${BUILD_COCOA})
    set(SET_OBJCXX_MANULLY OFF)
    if(CMAKE_VERSION VERSION_LESS 3.16)
        set(SET_OBJCXX_MANULLY ON)
    else()
        enable_language(OBJCXX)
    endif()
endif()

file(GLOB WV_HEADERS "include/xaml/ui/webview/*.hpp")

if(${BUILD_WINDOWS})
    file(GLOB WV_SOURCE "src/win/*.cpp")
elseif(${BUILD_COCOA})
    file(GLOB WV_SOURCE "src/cocoa/*.mm")
elseif(${BUILD_GTK3})
    file(GLOB WV_SOURCE "src/gtk3/*.cpp")
endif()

file(GLOB WV_SHARED_SOURCE "src/shared/*.cpp")

add_library(xaml_ui_webview ${WV_SOURCE} ${WV_SHARED_SOURCE})

target_include_directories(xaml_ui_webview
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${XAML_PRIVATE_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xaml_ui_webview xaml_ui)

set_target_properties(xaml_ui_webview PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION}
)

install(FILES ${WV_HEADERS} DESTINATION include/xaml/ui/webview)
install(TARGETS xaml_ui_webview EXPORT xaml_ui_webview-targets ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(EXPORT xaml_ui_webview-targets DESTINATION lib/cmake/xaml_ui_webview)
