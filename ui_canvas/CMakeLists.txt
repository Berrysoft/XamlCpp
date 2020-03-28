project(XamlCanvas CXX)

file(GLOB CV_HEADERS "include/xaml/ui/controls/*.hpp")

if(${BUILD_WINDOWS})
    option(BUILD_DIRECT2D "Build against Direct2D instead of GDI+" ON)
    file(GLOB CV_SOURCE "src/win/*.cpp")
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

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_ui_canvas PRIVATE "XAML_UI_CANVAS_API=__XAML_EXPORT" "GDIPVER=0x0110")
endif()

target_link_libraries(xaml_ui_canvas xaml_ui)
if(${BUILD_WINDOWS})
    if(${BUILD_DIRECT2D})
        target_compile_definitions(xaml_ui_canvas PUBLIC "XAML_UI_CANVAS_DIRECT2D")
    endif()
    target_link_libraries(xaml_ui_canvas gdiplus)

    # Use GDI+ 1.1
    set(USE_GDIP11_OPTIONS "/manifestdependency:type='Win32' name='Microsoft.Windows.GdiPlus' version='1.1.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'")
    if(CMAKE_VERSION VERSION_LESS 3.14)
        target_link_libraries(xaml_global INTERFACE ${USE_GDIP11_OPTIONS})
    else()
        target_link_options(xaml_global PUBLIC ${USE_GDIP11_OPTIONS})
    endif()
endif()
target_compile_definitions(xaml_ui_canvas PRIVATE "_USE_MATH_DEFINES")

install(FILES ${CV_HEADERS} DESTINATION include/xaml/ui/controls)

add_subdirectory(meta)
