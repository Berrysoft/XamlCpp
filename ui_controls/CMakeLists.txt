project(XamlUIControls CXX)

if(${BUILD_COCOA})
    set(SET_OBJCXX_MANULLY OFF)
    if(CMAKE_VERSION VERSION_LESS 3.16)
        set(SET_OBJCXX_MANULLY ON)
    else()
        enable_language(OBJCXX)
    endif()
endif()

file(GLOB UIC_HEADERS "include/xaml/ui/controls/*.hpp")

if(${BUILD_WINDOWS})
    file(GLOB UIC_SOURCE "src/win/*.cpp")
elseif(${BUILD_COCOA})
    file(GLOB UIC_SOURCE "src/cocoa/*.mm")
elseif(${BUILD_GTK3})
    file(GLOB UIC_SOURCE "src/gtk3/*.cpp")
endif()

file(GLOB UIC_SHARED_SOURCE "src/shared/*.cpp")

add_library(xaml_ui_controls ${UIC_SOURCE} ${UIC_SHARED_SOURCE})

target_include_directories(xaml_ui_controls
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${XAML_PRIVATE_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xaml_ui_controls xaml_ui)
if(${BUILD_WINDOWS})
    target_link_libraries(xaml_ui_controls gdiplus)
endif()

if(${USE_PCH})
    target_precompile_headers(xaml_ui_controls
        PUBLIC
            <$<BUILD_INTERFACE:xaml/ui/controls/button.hpp>>
            <$<BUILD_INTERFACE:xaml/ui/controls/canvas.hpp>>
            <$<BUILD_INTERFACE:xaml/ui/controls/entry.hpp>>
            <$<BUILD_INTERFACE:xaml/ui/controls/grid.hpp>>
            <$<BUILD_INTERFACE:xaml/ui/controls/label.hpp>>
            <$<BUILD_INTERFACE:xaml/ui/controls/password_entry.hpp>>
    )
endif()

set_target_properties(xaml_ui_controls PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION}
)

install(FILES ${UIC_HEADERS} DESTINATION include/xaml/ui/controls)
install(TARGETS xaml_ui_controls EXPORT xaml_ui_controls-targets ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(EXPORT xaml_ui_controls-targets DESTINATION lib/cmake/xaml_ui_controls)

configure_file(cmake/xaml_ui_controls-config.cmake.in ${PROJECT_BINARY_DIR}/xaml_ui_controls-config.cmake @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/xaml_ui_controls-config.cmake DESTINATION lib/cmake/xaml_ui_controls)

add_subdirectory(meta)

add_subdirectory(test)
