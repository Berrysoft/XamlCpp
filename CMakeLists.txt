cmake_minimum_required(VERSION 3.14)
project(XamlCpp VERSION 1.1.108 LANGUAGES C CXX)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")

include(XamlCompilerHelper)
xaml_check_ipo()

# Options for target platforms.
option(BUILD_WINDOWS "Build for Windows with Windows API")
option(BUILD_COCOA "Build for Mac with Cocoa")
option(BUILD_GTK3 "Build with GTK3")
option(BUILD_QT5 "Build with QT5")

# Select a platform automatically.
if(NOT ${BUILD_WINDOWS} AND NOT ${BUILD_COCOA} AND NOT ${BUILD_GTK3} AND NOT ${BUILD_QT5})
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

# Enable other languages.
if(${BUILD_COCOA})
    enable_language(OBJC OBJCXX)
    set(CMAKE_OBJC_STANDARD 11)
    set(CMAKE_OBJC_EXTENSIONS ON)
    set(CMAKE_OBJC_VISIBILITY_PRESET hidden)
    set(CMAKE_OBJCXX_STANDARD 20)
    set(CMAKE_OBJCXX_EXTENSIONS ON)
    set(CMAKE_OBJCXX_VISIBILITY_PRESET hidden)
endif()

set(XAML_BUILD_DEFINITIONS "")
set(XAML_BUILD_INTERNAL_DEFINITIONS "")
set(XAML_COMPILE_OPTIONS "")
set(XAML_C_COMPILE_OPTIONS "")
set(XAML_CXX_COMPILE_OPTIONS "")
set(XAML_OBJC_COMPILE_OPTIONS "")
set(XAML_LINK_OPTIONS "")

if(WIN32)
    list(APPEND XAML_BUILD_DEFINITIONS "XAML_WIN32")
    if(MINGW)
        list(APPEND XAML_BUILD_DEFINITIONS "XAML_MINGW")
    endif()
elseif(UNIX)
    list(APPEND XAML_BUILD_DEFINITIONS "XAML_UNIX")
    if(APPLE)
        list(APPEND XAML_BUILD_DEFINITIONS "XAML_APPLE")
    endif()
endif()

# Universal definitions, independent of compilers.
if(${BUILD_WINDOWS})
    list(APPEND XAML_BUILD_DEFINITIONS "UNICODE" "_UNICODE" "XAML_UI_WINDOWS")
    list(APPEND XAML_BUILD_INTERNAL_DEFINITIONS "_WIN32_WINNT=_WIN32_WINNT_WIN7")
elseif(${BUILD_COCOA})
    list(APPEND XAML_BUILD_DEFINITIONS "XAML_UI_COCOA")
    list(APPEND XAML_OBJC_COMPILE_OPTIONS "-fobjc-arc")
elseif(${BUILD_GTK3})
    list(APPEND XAML_BUILD_DEFINITIONS "XAML_UI_GTK3")
elseif(${BUILD_QT5})
    list(APPEND XAML_BUILD_DEFINITIONS "XAML_UI_QT5")
endif()

# Versions.
list(APPEND XAML_BUILD_DEFINITIONS 
    "XAML_VERSION=\"${CMAKE_PROJECT_VERSION}\"" 
    "XAML_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}" 
    "XAML_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}" 
    "XAML_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}"
)

# Big object for Windows.
if(MSVC)
    list(APPEND XAML_COMPILE_OPTIONS "/bigobj")
endif()

# Enable visual style on Windows.
if(MSVC)
    list(APPEND XAML_LINK_OPTIONS "/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'")
endif()

# Disable RTTI by default.
option(XAML_NO_RTTI "Disable RTTI." ON)
xaml_fix_rtti_flags()

if(${XAML_NO_RTTI})
    if(MSVC)
        list(APPEND XAML_CXX_COMPILE_OPTIONS "/GR-")
    else()
        list(APPEND XAML_CXX_COMPILE_OPTIONS "-fno-rtti")
    endif()
endif()

# Use absolute path for __FILE__
if(MSVC)
    list(APPEND XAML_COMPILE_OPTIONS "/FC")
endif()

# Disable char8_t
if(MSVC)
    list(APPEND XAML_CXX_COMPILE_OPTIONS "/std:c++latest" "/Zc:char8_t-")
else()
    list(APPEND XAML_CXX_COMPILE_OPTIONS "-fno-char8_t")
endif()

if(MINGW AND CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
    list(APPEND XAML_COMPILE_OPTIONS "-Wno-incompatible-ms-struct")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
    list(APPEND XAML_COMPILE_OPTIONS "-Wno-inconsistent-missing-override" "-Wno-unknown-attributes")
endif()

# Enable MSVC new preprocessor
#if(MSVC AND (NOT CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang"))
#    list(APPEND XAML_COMPILE_OPTIONS "/Zc:preprocessor" "/wd5104" "/wd5105")
#endif()

# MSVC warnings
xaml_fix_warning_flags()
if(MSVC)
    list(APPEND XAML_COMPILE_OPTIONS "/W4" "/wd4458")
endif()

option(BUILD_SHARED_LIBS "Build shared libs" ON)

if(NOT ${BUILD_SHARED_LIBS})
    list(APPEND XAML_BUILD_DEFINITIONS "XAML_STATIC_DEFINE")
    if(MSVC)
        # Set static library prefix to lib;
        # It is not recommanded to build static libraries.
        set(CMAKE_STATIC_LIBRARY_PREFIX_CXX lib)
    endif()
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(XAML_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR})
set(XAML_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR})
set(XAML_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR})

if(${BUILD_WINDOWS})
    find_package(WIL REQUIRED)
elseif(${BUILD_GTK3})
    find_package(GTK3 REQUIRED)
elseif(${BUILD_QT5})
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
endif()

option(BUILD_TESTS "Build tests." OFF)
option(INSTALL_RAPIDXML_HEADERS "Install (modified) rapidxml headers" OFF)
option(BUILD_PARSER "Build XAML parser." ON)
option(BUILD_WEBVIEW "Build webview." ON)
option(BUILD_CANVAS "Build canvas." ON)
option(BUILD_CONTROLS "Build controls." ON)
option(BUILD_CMDLINE "Build command line helpers." ON)
option(BUILD_DETECTOR "Build detector." ON)
option(BUILD_RESOURCE_COMPILER "Build resource compiler." ON)

unset(_XAML_BOOST_COMPONENTS)

if(APPLE)
    if(${BUILD_PARSER})
        set(_XAML_BOOST_COMPONENTS container)
    endif()
endif()

if(_XAML_BOOST_COMPONENTS OR ${BUILD_CMDLINE})
    if(_XAML_BOOST_COMPONENTS)
        find_package(Boost REQUIRED COMPONENTS ${_XAML_BOOST_COMPONENTS})
    else()
        find_package(Boost REQUIRED)
    endif()
endif()

set(_XAML_TARGETS
    xaml_global
    xaml_meta xaml_markup
    xaml_ui xaml_ui_appmain
)

option(XAML_INSTALL "Install XamlCpp components" ON)

add_subdirectory(StreamFormat)
list(APPEND _XAML_TARGETS stream_format)

add_subdirectory(nowide)
list(APPEND _XAML_TARGETS nowide)

if(${BUILD_PARSER})
    add_subdirectory(rapidxml)
    list(APPEND _XAML_TARGETS xaml_rapidxml)
endif()

xaml_fix_char8_t_flags()

add_subdirectory(global)
add_subdirectory(helpers)
add_subdirectory(meta)
add_subdirectory(markup)
add_subdirectory(ui)
if(${BUILD_PARSER})
    add_subdirectory(parser)
    list(APPEND _XAML_TARGETS xaml_parser)
endif()
if(${BUILD_CONTROLS})
    add_subdirectory(ui_controls)
    list(APPEND _XAML_TARGETS xaml_ui_controls)
endif()
if(${BUILD_CANVAS})
    add_subdirectory(ui_canvas)
    list(APPEND _XAML_TARGETS xaml_ui_canvas)
endif()
if(${BUILD_WEBVIEW})
    if(NOT ${BUILD_GTK3} OR (UNIX AND NOT APPLE))
        add_subdirectory(ui_webview)
        list(APPEND _XAML_TARGETS xaml_ui_webview)
    else()
        message(WARNING "webview won't be built because of unsupported platform.")
    endif()
endif()
if(${BUILD_CMDLINE})
    add_subdirectory(cmdline)
    list(APPEND _XAML_TARGETS xaml_cmdline)
endif()
if(${BUILD_DETECTOR} OR ${BUILD_RESOURCE_COMPILER})
    add_subdirectory(cmdline_helper)
    list(APPEND _XAML_TARGETS xaml_cmdline_helper)
endif()
if(${BUILD_DETECTOR})
    add_subdirectory(detector)
    list(APPEND _XAML_TARGETS xamld)
endif()
if(${BUILD_RESOURCE_COMPILER})
    add_subdirectory(resource)
    add_subdirectory(resource_compiler)
    list(APPEND _XAML_TARGETS xamlrc)
endif()

if(${XAML_INSTALL})
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
endif()
