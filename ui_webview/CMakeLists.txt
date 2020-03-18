project(XamlWebView CXX)

if(${BUILD_COCOA})
    set(SET_OBJCXX_MANULLY OFF)
    if(CMAKE_VERSION VERSION_LESS 3.16)
        set(SET_OBJCXX_MANULLY ON)
    else()
        enable_language(OBJCXX)
    endif()
endif()

file(GLOB WV_HEADERS "include/xaml/ui/controls/*.hpp")

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
    PRIVATE
        ${XAML_PRIVATE_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xaml_ui_webview xaml_ui)
if(${BUILD_WINDOWS})
    option(BUILD_WEBVIEW2 "Build webview2 which uses Edge(Chromium)." ON)
    option(BUILD_EDGE "Build webview which uses Edge." ON)

    target_link_libraries(xaml_ui_webview ShLwApi)

    if(${BUILD_WEBVIEW2})
        target_compile_definitions(xaml_ui_webview PRIVATE "-DXAML_UI_WEBVIEW_WEBVIEW2")

        if(NOT EXISTS "${CMAKE_BINARY_DIR}/webview2.nupkg")
            message(STATUS "Download WebView2...")
            file(DOWNLOAD https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/0.9.430 ${CMAKE_BINARY_DIR}/webview2.nupkg)
            message(STATUS "WebView2 downloaded")
        endif()

        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/webview2)
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_BINARY_DIR}/webview2.nupkg
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/webview2
        )

        if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
            set(XAML_NATIVE_PLATFORM "x86")
        else()
            set(XAML_NATIVE_PLATFORM "x64")
        endif()
        target_include_directories(xaml_ui_webview PRIVATE ${CMAKE_BINARY_DIR}/webview2/build/native/include)
        target_link_libraries(xaml_ui_webview "${CMAKE_BINARY_DIR}/webview2/build/native/${XAML_NATIVE_PLATFORM}/WebView2Loader.dll.lib")
        configure_file(${CMAKE_BINARY_DIR}/webview2/build/native/${XAML_NATIVE_PLATFORM}/WebView2Loader.dll ${CMAKE_BINARY_DIR}/bin/WebView2Loader.dll COPYONLY)
    endif()

    if(${BUILD_EDGE})
        target_compile_definitions(xaml_ui_webview PRIVATE "-DXAML_UI_WEBVIEW_EDGE")

        if(NOT EXISTS "${CMAKE_BINARY_DIR}/cppwinrt.nupkg")
            message(STATUS "Downloading CppWinRT...")
            file(DOWNLOAD https://www.nuget.org/api/v2/package/Microsoft.Windows.CppWinRT/2.0.200316.3 ${CMAKE_BINARY_DIR}/cppwinrt.nupkg)
            message(STATUS "CppWinRT downloaded")
        endif()

        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/cppwinrt)
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_BINARY_DIR}/cppwinrt.nupkg
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/cppwinrt
        )

        execute_process(COMMAND ${CMAKE_BINARY_DIR}/cppwinrt/bin/cppwinrt.exe -in local -out ${CMAKE_BINARY_DIR}/cppwinrt/gen/)

        target_include_directories(xaml_ui_webview PRIVATE ${CMAKE_BINARY_DIR}/cppwinrt/gen)
    endif()
elseif(${BUILD_COCOA})
    target_link_libraries(xaml_ui_webview "-framework WebKit")
elseif(${BUILD_GTK3})
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WEBKIT4 REQUIRED webkit2gtk-4.0)
    link_directories(${WEBKIT4_LIBRARY_DIRS})
    add_definitions(${WEBKIT4_CFLAGS_OTHER})

    target_include_directories(xaml_ui_webview PRIVATE ${WEBKIT4_INCLUDE_DIRS})
    target_link_libraries(xaml_ui_webview ${WEBKIT4_LIBRARIES})
endif()

install(FILES ${WV_HEADERS} DESTINATION include/xaml/ui/controls)

add_subdirectory(meta)

add_subdirectory(test)
