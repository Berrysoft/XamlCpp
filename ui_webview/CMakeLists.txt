project(XamlWebView CXX)

file(GLOB WV_HEADERS "include/xaml/ui/controls/*.h")

if(${BUILD_WINDOWS})
    file(GLOB WV_SOURCE "src/win/*.c*")
elseif(${BUILD_COCOA})
    file(GLOB WV_SOURCE "src/cocoa/*.mm")
elseif(${BUILD_GTK3})
    file(GLOB WV_SOURCE "src/gtk3/*.cpp")
elseif(${BUILD_QT5})
    file(GLOB WV_SOURCE "src/qt5/*.cpp")
endif()

file(GLOB WV_SHARED_SOURCE "src/shared/*.cpp")

add_library(xaml_ui_webview ${WV_SOURCE} ${WV_SHARED_SOURCE})

target_include_directories(xaml_ui_webview
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_ui_webview PRIVATE "XAML_UI_WEBVIEW_API=__XAML_EXPORT")
endif()

target_link_libraries(xaml_ui_webview PUBLIC xaml_ui)
if(${BUILD_WINDOWS})
    option(BUILD_WEBVIEW2 "Build target webview2 which uses Edge(Chromium)." ON)
    option(BUILD_EDGE "Build target webview which uses Edge." ON)

    target_link_libraries(xaml_ui_webview PUBLIC ShLwApi PRIVATE wil xaml_helpers)

    if(${BUILD_WEBVIEW2})
        target_compile_definitions(xaml_ui_webview PRIVATE "XAML_UI_WEBVIEW_WEBVIEW2")

        if(NOT EXISTS "${CMAKE_BINARY_DIR}/webview2.nupkg")
            message(STATUS "Downloading WebView2...")
            file(DOWNLOAD https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.674-prerelease ${CMAKE_BINARY_DIR}/webview2.nupkg)
            message(STATUS "Downloading WebView2... done")
        endif()

        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/webview2)
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_BINARY_DIR}/webview2.nupkg
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/webview2
        )

        if(${CMAKE_SIZEOF_VOID_P} EQUAL 4)
            set(XAML_NATIVE_PLATFORM "x86")
        else()
            set(XAML_NATIVE_PLATFORM "x64")
        endif()
        target_include_directories(xaml_ui_webview PRIVATE ${CMAKE_BINARY_DIR}/webview2/build/native/include)
        target_link_libraries(xaml_ui_webview PRIVATE "${CMAKE_BINARY_DIR}/webview2/build/native/${XAML_NATIVE_PLATFORM}/WebView2Loader.dll.lib")
        configure_file(${CMAKE_BINARY_DIR}/webview2/build/native/${XAML_NATIVE_PLATFORM}/WebView2Loader.dll ${XAML_RUNTIME_OUTPUT_DIRECTORY}/WebView2Loader.dll COPYONLY)
        if(${XAML_INSTALL})
            install(FILES ${XAML_RUNTIME_OUTPUT_DIRECTORY}/WebView2Loader.dll DESTINATION bin)
        endif()
    endif()

    if(${BUILD_EDGE})
        target_compile_definitions(xaml_ui_webview PRIVATE "XAML_UI_WEBVIEW_EDGE")
    endif()
elseif(${BUILD_COCOA})
    target_link_libraries(xaml_ui_webview PUBLIC "-framework WebKit")
elseif(${BUILD_GTK3})
    target_link_libraries(xaml_ui_webview PRIVATE gtk3 webkit4)
elseif(${BUILD_QT5})
    option(BUILD_WEBKIT "Build target Qt5::WebKitWidgets." OFF)

    if(${BUILD_WEBKIT})
        target_compile_definitions(xaml_ui_webview PRIVATE "XAML_UI_WEBVIEW_WEBKIT")
        target_link_libraries(xaml_ui_webview PRIVATE Qt5::WebKitWidgets)
    else()
        target_link_libraries(xaml_ui_webview PRIVATE Qt5::WebEngineWidgets)
    endif()
endif()

if(${XAML_INSTALL})
    install(FILES ${WV_HEADERS} DESTINATION include/xaml/ui/controls)
endif()

if(${BUILD_TESTS})
    add_subdirectory(test)
endif()
