cmake_minimum_required(VERSION 3.7)
project(XamlCpp VERSION 1.0.19 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")

option(BUILD_WINDOWS "Build for Windows with Windows API")
option(BUILD_COCOA "Build for Mac with Cocoa")
option(BUILD_GTK3 "Build with GTK3")

if(NOT ${BUILD_WINDOWS} AND NOT ${BUILD_COCOA} AND NOT ${BUILD_GTK3})
    if(WIN32 AND NOT MINGW)
        set(BUILD_WINDOWS ON)
    elseif(UNIX OR MINGW)
        if(APPLE)
            set(BUILD_COCOA ON)
        else()
            set(BUILD_GTK3 ON)
        endif()
    endif()
endif()

set(XAML_BUILD_DEFINITIONS "")
set(XAML_COMPILE_OPTIONS "")
set(XAML_LINK_OPTIONS "")

if(${BUILD_WINDOWS})
    set(XAML_BUILD_DEFINITIONS "-DUNICODE" "-D_UNICODE" "-DXAML_UI_WINDOWS" "-DVC_EXTRALEAN")
    set(XAML_COMPILE_OPTIONS "")
    set(XAML_LINK_OPTIONS "")
elseif(${BUILD_COCOA})
    set(XAML_BUILD_DEFINITIONS "-DXAML_UI_COCOA")
    set(XAML_COMPILE_OPTIONS "-fobjc-arc")
    set(XAML_LINK_OPTIONS "")
elseif(${BUILD_GTK3})
    set(XAML_BUILD_DEFINITIONS "-DXAML_UI_GTK3" "-D_USE_MATH_DEFINES")
    set(XAML_COMPILE_OPTIONS "")
    set(XAML_LINK_OPTIONS "")
endif()

list(APPEND XAML_COMPILE_OPTIONS "-DXAML_VERSION=\"${CMAKE_PROJECT_VERSION}\"")

if(MSVC)
    list(APPEND XAML_COMPILE_OPTIONS "/bigobj")
    list(APPEND XAML_LINK_OPTIONS "/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'")
elseif(MINGW)
    list(APPEND XAML_COMPILE_OPTIONS "-Wa,-mbig-obj")
endif()

if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
    if(MINGW)
        list(APPEND XAML_COMPILE_OPTIONS "-O1")
    endif()
else()
    if(MSVC)
        list(APPEND XAML_COMPILE_OPTIONS "/GL")
        list(APPEND XAML_LINK_OPTIONS "/LTCG")
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" AND NOT MINGW)
        list(APPEND XAML_COMPILE_OPTIONS "-flto")
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        list(APPEND XAML_COMPILE_OPTIONS "-flto")
        list(APPEND XAML_LINK_OPTIONS "-flto")
        if(UNIX AND NOT APPLE)
            list(APPEND XAML_LINK_OPTIONS "-fuse-ld=gold")
        endif()
    endif()
endif()

option(BUILD_SHARED_LIBS "Build shared libs" ON)

if(NOT ${BUILD_SHARED_LIBS})
    list(APPEND XAML_BUILD_DEFINITIONS "-DXAML_STATIC_DEFINE")
    if(MSVC)
        # set static library prefix to lib
        set(CMAKE_STATIC_LIBRARY_PREFIX_CXX lib)
    endif()
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(XAML_PRIVATE_INCLUDE_DIRS )

if(${BUILD_WINDOWS})
    find_package(WIL REQUIRED)
    set(XAML_PRIVATE_INCLUDE_DIRS ${WIL_INCLUDE_DIRS})
elseif(${BUILD_GTK3})
    find_package(GTK3 REQUIRED)
    set(XAML_PRIVATE_INCLUDE_DIRS ${GTK3_INCLUDE_DIRS})
endif()

option(BUILD_WEBVIEW "Build webview." ON)

set(_XAML_TARGETS xaml_meta xaml_ui xaml_ui_meta xaml_ui_controls xaml_ui_controls_meta xaml xamlc)

add_subdirectory(meta)
add_subdirectory(ui)
add_subdirectory(ui_controls)
if(${BUILD_WEBVIEW})
    if(NOT ${BUILD_GTK3} OR (UNIX AND NOT APPLE))
        add_subdirectory(ui_webview)
        list(APPEND _XAML_TARGETS xaml_ui_webview xaml_ui_webview_meta)
    else()
        message(WARNING "webview won't be built because of unsupported platform.")
    endif()
endif()
add_subdirectory(xaml)
add_subdirectory(xamlc)
add_subdirectory(xamlcpp)

install(
    TARGETS ${_XAML_TARGETS}
    EXPORT xaml-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(EXPORT xaml-targets DESTINATION lib/cmake/xaml)
configure_file(cmake/xaml-config.cmake.in ${PROJECT_BINARY_DIR}/xaml-config.cmake @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/xaml-config.cmake DESTINATION lib/cmake/xaml)
