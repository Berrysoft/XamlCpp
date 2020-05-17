project(XamlUI)

file(GLOB UI_HEADERS "include/xaml/ui/*.h*")
file(GLOB UI_WINDOWS_HEADERS "include/xaml/ui/win/*.h")
file(GLOB UI_COCOA_HEADERS "include/xaml/ui/cocoa/*.h")
file(GLOB UI_GTK3_HEADERS "include/xaml/ui/gtk3/*.h")

if(${BUILD_WINDOWS})
    file(GLOB UI_SOURCE "src/win/*.c*")
elseif(${BUILD_COCOA})
    file(GLOB UI_SOURCE "src/cocoa/*.m*")
elseif(${BUILD_GTK3})
    file(GLOB UI_SOURCE "src/gtk3/*.c*")
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

target_link_libraries(xaml_ui PUBLIC xaml_meta)
if(${BUILD_WINDOWS})
    target_link_libraries(xaml_ui PUBLIC Comctl32 ntdll UxTheme Dwmapi PRIVATE wil)
elseif(${BUILD_COCOA})
    target_link_libraries(xaml_ui PUBLIC "-framework Cocoa")
elseif(${BUILD_GTK3})
    target_link_libraries(xaml_ui PRIVATE gtk3)
endif()

if(Boost_nowide_FOUND AND ${BUILD_GTK3})
    target_link_libraries(xaml_ui PRIVATE Boost::nowide)
endif()

install(FILES ${UI_HEADERS} DESTINATION include/xaml/ui)
install(FILES ${UI_WINDOWS_HEADERS} DESTINATION include/xaml/ui/win)
install(FILES ${UI_COCOA_HEADERS} DESTINATION include/xaml/ui/cocoa)
install(FILES ${UI_GTK3_HEADERS} DESTINATION include/xaml/ui/gtk3)

add_subdirectory(appmain)
