cmake_minimum_required(VERSION 3.7)
project(XamlCpp VERSION 1.0.72 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
    set(CMAKE_OBJCXX_STANDARD 17)
    set(CMAKE_OBJCXX_EXTENSIONS ON)
elseif(${BUILD_GTK3})
    enable_language(C)
    set(CMAKE_C_STANDARD 99)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)
endif()

set(XAML_BUILD_DEFINITIONS "")
set(XAML_COMPILE_OPTIONS "")
set(XAML_LINK_OPTIONS "")

# Universal definitions, independent of compilers.
if(${BUILD_WINDOWS})
    list(APPEND XAML_BUILD_DEFINITIONS "-DUNICODE" "-D_UNICODE" "-DXAML_UI_WINDOWS")

    if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
        list(APPEND XAML_BUILD_DEFINITIONS "-D_WIN32_WINNT=_WIN32_WINNT_WIN7")
    endif()
elseif(${BUILD_COCOA})
    list(APPEND XAML_BUILD_DEFINITIONS "-DXAML_UI_COCOA")
    list(APPEND XAML_COMPILE_OPTIONS "-fobjc-arc")
elseif(${BUILD_GTK3})
    list(APPEND XAML_BUILD_DEFINITIONS "-DXAML_UI_GTK3")
endif()

# Versions.
list(APPEND XAML_COMPILE_OPTIONS 
    "-DXAML_VERSION=\"${CMAKE_PROJECT_VERSION}\"" 
    "-DXAML_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}" 
    "-DXAML_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}" 
    "-DXAML_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}"
)

# Big object for Windows.
if(MSVC)
    list(APPEND XAML_COMPILE_OPTIONS "/bigobj")
    list(APPEND XAML_LINK_OPTIONS "/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'")
elseif(MINGW)
    list(APPEND XAML_COMPILE_OPTIONS "-Wa,-mbig-obj")
endif()

# Set warnings.
if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        list(APPEND XAML_COMPILE_OPTIONS "/W4")
    endif()
    list(APPEND XAML_COMPILE_OPTIONS "/wd4100")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    list(APPEND XAML_COMPILE_OPTIONS "-Wall")
endif()

# LTO and work around for MinGW.
if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
    if(MINGW)
        list(APPEND XAML_COMPILE_OPTIONS "-O1")
    endif()
elseif(NOT MINGW)
    if(MSVC)
        list(APPEND XAML_COMPILE_OPTIONS "/GL")
        list(APPEND XAML_LINK_OPTIONS "/LTCG")
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
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
        # Set static library prefix to lib;
        # It is not recommanded to build static libraries.
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
option(USE_SYS_PUGIXML "Use system pugixml" OFF)

set(_XAML_TARGETS xaml_global xaml_meta xaml_ui xaml_ui_meta xaml_ui_controls xaml_ui_controls_meta xaml_ui_canvas xaml_ui_canvas_meta xaml_parser xaml_compiler)

if(${USE_SYS_PUGIXML})
    find_package(pugixml REQUIRED)
else()
    add_subdirectory(pugixml)
    list(APPEND _XAML_TARGETS xaml_pugixml)
endif()

add_subdirectory(global)
add_subdirectory(meta)
add_subdirectory(ui)
add_subdirectory(ui_controls)
add_subdirectory(ui_canvas)
if(${BUILD_WEBVIEW})
    if(NOT ${BUILD_GTK3} OR (UNIX AND NOT APPLE))
        add_subdirectory(ui_webview)
        list(APPEND _XAML_TARGETS xaml_ui_webview xaml_ui_webview_meta)
    else()
        message(WARNING "webview won't be built because of unsupported platform.")
    endif()
endif()
add_subdirectory(parser)
add_subdirectory(compiler)
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
