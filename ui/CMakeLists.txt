project(XamlUI)

file(GLOB UI_HEADERS "include/xaml/ui/*.h*")
file(GLOB UI_WINDOWS_HEADERS "include/xaml/ui/win/*.h")
file(GLOB UI_COCOA_HEADERS "include/xaml/ui/cocoa/*.h")
file(GLOB UI_GTK3_HEADERS "include/xaml/ui/gtk3/*.h")
file(GLOB UI_QT5_HEADERS "include/xaml/ui/qt5/*.h")

if(${BUILD_WINDOWS})
    file(GLOB UI_SOURCE "src/win/*.c*")
elseif(${BUILD_COCOA})
    file(GLOB UI_SOURCE "src/cocoa/*.m*")
elseif(${BUILD_GTK3})
    file(GLOB UI_SOURCE "src/gtk3/*.c*")
elseif(${BUILD_QT5})
    file(GLOB UI_SOURCE "src/qt5/*.cpp")
endif()

file(GLOB UI_SHARED_SOURCE "src/shared/*.cpp")

add_library(xaml_ui ${UI_SOURCE} ${UI_SHARED_SOURCE})

target_include_directories(xaml_ui
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_ui PRIVATE "XAML_UI_API=__XAML_EXPORT")
endif()

target_link_libraries(xaml_ui PUBLIC xaml_markup PRIVATE nowide)
if(${BUILD_WINDOWS})
    target_link_libraries(xaml_ui PRIVATE Comctl32 ntdll UxTheme Dwmapi wil xaml_helpers)
elseif(${BUILD_COCOA})
    target_link_libraries(xaml_ui PUBLIC "-framework Cocoa")
elseif(${BUILD_GTK3})
    target_link_libraries(xaml_ui PRIVATE gtk3)
elseif(${BUILD_QT5})
    target_link_libraries(xaml_ui PRIVATE Qt5::Widgets)
endif()

if(${XAML_INSTALL})
    install(FILES ${UI_HEADERS} DESTINATION include/xaml/ui)
    install(FILES ${UI_WINDOWS_HEADERS} DESTINATION include/xaml/ui/win)
    install(FILES ${UI_COCOA_HEADERS} DESTINATION include/xaml/ui/cocoa)
    install(FILES ${UI_GTK3_HEADERS} DESTINATION include/xaml/ui/gtk3)
    install(FILES ${UI_QT5_HEADERS} DESTINATION include/xaml/ui/qt5)
endif()

add_subdirectory(appmain)
