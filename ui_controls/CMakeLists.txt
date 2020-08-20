project(XamlUIControls CXX)

file(GLOB UIC_HEADERS "include/xaml/ui/controls/*.h")
file(GLOB UIC_WINDOWS_HEADERS "include/xaml/ui/win/controls/*.h")
file(GLOB UIC_COCOA_HEADERS "include/xaml/ui/cocoa/controls/*.h")
file(GLOB UIC_GTK3_HEADERS "include/xaml/ui/gtk3/controls/*.h")

if(${BUILD_WINDOWS})
    file(GLOB UIC_SOURCE "src/win/*.cpp")
elseif(${BUILD_COCOA})
    file(GLOB UIC_SOURCE "src/cocoa/*.mm")
elseif(${BUILD_GTK3})
    file(GLOB UIC_SOURCE "src/gtk3/*.cpp")
elseif(${BUILD_QT5})
    file(GLOB UIC_SOURCE "src/qt5/*.cpp")
endif()

file(GLOB UIC_SHARED_SOURCE "src/shared/*.cpp")

add_library(xaml_ui_controls ${UIC_SOURCE} ${UIC_SHARED_SOURCE})

target_include_directories(xaml_ui_controls
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_ui_controls PRIVATE "XAML_UI_CONTROLS_API=__XAML_EXPORT")
endif()

target_link_libraries(xaml_ui_controls PUBLIC xaml_ui)
if(${BUILD_WINDOWS})
    target_link_libraries(xaml_ui_controls PRIVATE wil xaml_helpers)
elseif(${BUILD_GTK3})
    target_link_libraries(xaml_ui_controls PRIVATE gtk3)
elseif(${BUILD_QT5})
    target_link_libraries(xaml_ui_controls PRIVATE Qt5::Widgets)
endif()

if(${XAML_INSTALL})
    install(FILES ${UIC_HEADERS} DESTINATION include/xaml/ui/controls)
    install(FILES ${UIC_WINDOWS_HEADERS} DESTINATION include/xaml/ui/win/controls)
    install(FILES ${UIC_COCOA_HEADERS} DESTINATION include/xaml/ui/cocoa/controls)
    install(FILES ${UIC_GTK3_HEADERS} DESTINATION include/xaml/ui/gtk3/controls)
endif()

if(${BUILD_TESTS})
    add_subdirectory(test)
endif()
