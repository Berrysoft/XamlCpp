project(XamlCanvas CXX)

file(GLOB CV_HEADERS "include/xaml/ui/controls/*.h")
file(GLOB CV_WINDOWS_HEADERS "include/xaml/ui/win/controls/*.h")
file(GLOB CV_COCOA_HEADERS "include/xaml/ui/cocoa/controls/*.h")
file(GLOB CV_GTK3_HEADERS "include/xaml/ui/gtk3/controls/*.h")

if(${BUILD_WINDOWS})
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
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_ui_canvas PRIVATE "XAML_UI_CANVAS_API=__XAML_EXPORT" "GDIPVER=0x0110")
endif()

target_link_libraries(xaml_ui_canvas PUBLIC xaml_ui)
if(${BUILD_WINDOWS})
    target_link_libraries(xaml_ui_canvas PUBLIC gdiplus d2d1 dwrite PRIVATE wil xaml_helpers)
elseif(${BUILD_GTK3})
    target_link_libraries(xaml_ui_canvas PRIVATE gtk3)
endif()

target_compile_definitions(xaml_ui_canvas PRIVATE "_USE_MATH_DEFINES")

install(FILES ${CV_HEADERS} DESTINATION include/xaml/ui/controls)
install(FILES ${CV_WINDOWS_HEADERS} DESTINATION include/xaml/ui/win/controls)
install(FILES ${CV_COCOA_HEADERS} DESTINATION include/xaml/ui/cocoa/controls)
install(FILES ${CV_GTK3_HEADERS} DESTINATION include/xaml/ui/gtk3/controls)
