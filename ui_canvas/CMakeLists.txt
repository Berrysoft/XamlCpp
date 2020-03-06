project(XamlCanvas CXX)

if(${BUILD_COCOA})
    set(SET_OBJCXX_MANULLY OFF)
    if(CMAKE_VERSION VERSION_LESS 3.16)
        set(SET_OBJCXX_MANULLY ON)
    else()
        enable_language(OBJCXX)
    endif()
endif()

file(GLOB CV_HEADERS "include/xaml/ui/controls/*.hpp")

if(${BUILD_WINDOWS})
    option(BUILD_DIRECT2D "Build against Direct2D instead of GDI+" OFF)
    if(${BUILD_DIRECT2D})
        file(GLOB CV_SOURCE "src/win-d2d/*.cpp")
    else()
        file(GLOB CV_SOURCE "src/win/*.cpp")
    endif()
elseif(${BUILD_COCOA})
    file(GLOB CV_SOURCE "src/cocoa/*.mm")
elseif(${BUILD_GTK3})
    file(GLOB CV_SOURCE "src/gtk3/*.cpp")
endif()

file(GLOB CV_SHARED_SOURCE "src/shared/*.cpp")

add_library(xaml_ui_canvas ${CV_SOURCE} ${CV_SHARED_SOURCE})

target_include_directories(xaml_ui_canvas
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${XAML_PRIVATE_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xaml_ui_canvas xaml_ui)
if(${BUILD_WINDOWS})
    target_link_libraries(xaml_ui_canvas gdiplus)
endif()

set_target_properties(xaml_ui_canvas PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION}
)

install(FILES ${CV_HEADERS} DESTINATION include/xaml/ui/controls)

add_subdirectory(meta)
