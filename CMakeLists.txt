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

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")

# Options for target platforms.
option(BUILD_WINDOWS "Build for Windows with Windows API")
option(BUILD_COCOA "Build for Mac with Cocoa")
option(BUILD_GTK3 "Build with GTK3")

# Select a platform automatically.
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
    list(APPEND XAML_COMPILE_OPTIONS "-fobjc-arc")
elseif(${BUILD_GTK3})
    list(APPEND XAML_BUILD_DEFINITIONS "XAML_UI_GTK3")
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
    list(APPEND XAML_LINK_OPTIONS "/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'")
elseif(MINGW)
    list(APPEND XAML_COMPILE_OPTIONS "-Wa,-mbig-obj")
endif()

# Disable RTTI by default.
option(XAML_NO_RTTI "Disable RTTI." ON)

if(${XAML_NO_RTTI})
    if(MSVC)
        string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        list(APPEND XAML_COMPILE_OPTIONS "/GR-")
    else()
        string(REGEX REPLACE "-frtti" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        list(APPEND XAML_COMPILE_OPTIONS "-fno-rtti")
    endif()
endif()

if(MINGW AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND XAML_COMPILE_OPTIONS "-Wno-incompatible-ms-struct")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND XAML_COMPILE_OPTIONS "-Wno-inconsistent-missing-override")
endif()

# Enable MSVC new preprocessor
#if(MSVC AND (NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
#    list(APPEND XAML_COMPILE_OPTIONS "/Zc:preprocessor" "/wd5104" "/wd5105")
#endif()

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

if(${BUILD_WINDOWS})
    find_package(WIL REQUIRED)
elseif(${BUILD_GTK3})
    find_package(GTK3 REQUIRED)
endif()

if(APPLE)
    find_package(Boost REQUIRED iostreams container)
endif()

option(BUILD_TESTS "Build tests." ON)
option(INSTALL_RAPIDXML_HEADERS "Install (modified) rapidxml headers" ON)
option(BUILD_WEBVIEW "Build webview." ON)

set(_XAML_TARGETS
    xaml_global xaml_rapidxml
    xaml_meta xaml_parser
    xaml_ui xaml_ui_appmain
    xaml_ui_controls
    xaml_ui_canvas
    xaml_cmdline
)

add_subdirectory(global)
add_subdirectory(helpers)
add_subdirectory(meta)
add_subdirectory(rapidxml)
add_subdirectory(parser)
add_subdirectory(ui)
add_subdirectory(ui_controls)
add_subdirectory(ui_canvas)
if(${BUILD_WEBVIEW})
    if(NOT ${BUILD_GTK3} OR (UNIX AND NOT APPLE))
        add_subdirectory(ui_webview)
        list(APPEND _XAML_TARGETS xaml_ui_webview)
    else()
        message(WARNING "webview won't be built because of unsupported platform.")
    endif()
endif()
add_subdirectory(cmdline)
add_subdirectory(detector)
add_subdirectory(resource)

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
