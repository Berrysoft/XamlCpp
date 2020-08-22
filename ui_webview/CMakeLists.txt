project(XamlWebView CXX)

file(GLOB WV_HEADERS "include/xaml/ui/controls/*.h")

if(${BUILD_WINDOWS})
    file(GLOB WV_SOURCE "src/win/*.cpp")
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
    option(BUILD_WEBVIEW2 "Build webview2 which uses Edge(Chromium)." ON)
    option(BUILD_EDGE "Build webview which uses Edge." ON)

    target_link_libraries(xaml_ui_webview PUBLIC ShLwApi PRIVATE wil xaml_helpers)

    if(${BUILD_WEBVIEW2})
        target_compile_definitions(xaml_ui_webview PRIVATE "XAML_UI_WEBVIEW_WEBVIEW2")

        if(NOT EXISTS "${CMAKE_BINARY_DIR}/webview2.nupkg")
            message(STATUS "Downloading WebView2...")
            file(DOWNLOAD https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/0.9.579 ${CMAKE_BINARY_DIR}/webview2.nupkg)
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

        if(NOT EXISTS "${CMAKE_BINARY_DIR}/cppwinrt.nupkg")
            message(STATUS "Downloading CppWinRT...")
            file(DOWNLOAD https://www.nuget.org/api/v2/package/Microsoft.Windows.CppWinRT/2.0.200729.8 ${CMAKE_BINARY_DIR}/cppwinrt.nupkg)
            message(STATUS "Downloading CppWinRT... done")
        endif()

        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/cppwinrt)
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_BINARY_DIR}/cppwinrt.nupkg
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/cppwinrt
        )

        execute_process(COMMAND ${CMAKE_BINARY_DIR}/cppwinrt/bin/cppwinrt.exe -in local -out ${CMAKE_BINARY_DIR}/cppwinrt/gen/)

        target_include_directories(xaml_ui_webview PRIVATE ${CMAKE_BINARY_DIR}/cppwinrt/gen)
        target_compile_options(xaml_ui_webview PRIVATE "/await")
    endif()
elseif(${BUILD_COCOA})
    target_link_libraries(xaml_ui_webview PUBLIC "-framework WebKit")
elseif(${BUILD_GTK3})
    find_package(WEBKIT4 REQUIRED)

    target_link_libraries(xaml_ui_webview PRIVATE gtk3 webkit4)
elseif(${BUILD_QT5})
    target_link_libraries(xaml_ui_webview PRIVATE Qt5::WebEngineWidgets)
endif()

if(${XAML_INSTALL})
    install(FILES ${WV_HEADERS} DESTINATION include/xaml/ui/controls)
endif()

if(${BUILD_TESTS})
    add_subdirectory(test)
endif()
