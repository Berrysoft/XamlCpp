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
if(${BUILD_WINDOWS})
    target_link_libraries(xaml_ui_webview WindowsApp)

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/webview2.nupkg")
        file(DOWNLOAD https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/0.8.355 ${CMAKE_BINARY_DIR}/webview2.nupkg)
    endif()

    add_custom_target(unzip_webview2 ALL)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/webview2)
    add_custom_command(TARGET unzip_webview2 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_BINARY_DIR}/webview2.nupkg
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/webview2
        DEPENDS ${CMAKE_BINARY_DIR}/webview2.nupkg
        COMMENT "Unpacking webview2.nupkg"
        VERBATIM
    )

    add_dependencies(xaml_ui_webview unzip_webview2)

    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        set(XAML_NATIVE_PLATFORM "x86")
    else()
        set(XAML_NATIVE_PLATFORM "x64")
    endif()
    target_include_directories(xaml_ui_webview PRIVATE ${CMAKE_BINARY_DIR}/webview2/build/native/include)
    file(GLOB WEBVIEW2_LIBS "${CMAKE_BINARY_DIR}/webview2/build/native/${XAML_NATIVE_PLATFORM}/*.lib")
    target_link_libraries(xaml_ui_webview ${WEBVIEW2_LIBS})
    configure_file(${CMAKE_BINARY_DIR}/webview2/build/native/${XAML_NATIVE_PLATFORM}/WebView2Loader.dll ${CMAKE_BINARY_DIR}/bin/WebView2Loader.dll COPYONLY)
elseif(${BUILD_GTK3})
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WEBKIT4 REQUIRED webkit2gtk-4.0)
    link_directories(${WEBKIT4_LIBRARY_DIRS})
    add_definitions(${WEBKIT4_CFLAGS_OTHER})

    target_include_directories(xaml_ui_webview PRIVATE ${WEBKIT4_INCLUDE_DIRS})
    target_link_libraries(xaml_ui_webview ${WEBKIT4_LIBRARIES})
endif()

set_target_properties(xaml_ui_webview PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION}
)

install(FILES ${WV_HEADERS} DESTINATION include/xaml/ui/webview)
install(TARGETS xaml_ui_webview EXPORT xaml_ui_webview-targets ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(EXPORT xaml_ui_webview-targets DESTINATION lib/cmake/xaml_ui_webview)

add_subdirectory(test)