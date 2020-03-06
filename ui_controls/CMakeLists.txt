project(XamlUIControls CXX)

if(${BUILD_COCOA})
    set(SET_OBJCXX_MANULLY OFF)
    if(CMAKE_VERSION VERSION_LESS 3.16)
        set(SET_OBJCXX_MANULLY ON)
    else()
        enable_language(OBJCXX)
    endif()
endif()

if(${BUILD_GTK3})
    enable_language(C)
endif()

file(GLOB UIC_HEADERS "include/xaml/ui/controls/*.hpp")

if(${BUILD_WINDOWS})
    file(GLOB UIC_SOURCE "src/win/*.cpp")
elseif(${BUILD_COCOA})
    file(GLOB UIC_SOURCE "src/cocoa/*.mm")
elseif(${BUILD_GTK3})
    file(GLOB UIC_SOURCE "src/gtk3/*.c" "src/gtk3/*.cpp")
endif()

file(GLOB UIC_SHARED_SOURCE "src/shared/*.cpp")

add_library(xaml_ui_controls ${UIC_SOURCE} ${UIC_SHARED_SOURCE})

target_include_directories(xaml_ui_controls
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${XAML_PRIVATE_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xaml_ui_controls xaml_ui)

set_target_properties(xaml_ui_controls PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION}
)

install(FILES ${UIC_HEADERS} DESTINATION include/xaml/ui/controls)

add_subdirectory(meta)

add_subdirectory(test)
