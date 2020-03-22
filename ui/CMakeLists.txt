project(XamlUI CXX)

file(GLOB UI_HEADERS "include/xaml/ui/*.hpp")
file(GLOB UI_COCOA_HEADERS "include/xaml/ui/cocoa/*.h")

if(${BUILD_WINDOWS})
    file(GLOB UI_SOURCE "src/win/*.cpp")
elseif(${BUILD_COCOA})
    file(GLOB UI_SOURCE "src/cocoa/*.m" "src/cocoa/*.mm")
elseif(${BUILD_GTK3})
    file(GLOB UI_SOURCE "src/gtk3/*.cpp")
endif()

file(GLOB UI_SHARED_SOURCE "src/shared/*.cpp")

add_library(xaml_ui ${UI_SOURCE} ${UI_SHARED_SOURCE})

target_include_directories(xaml_ui
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    PRIVATE
        ${XAML_PRIVATE_INCLUDE_DIRS}
)

target_link_libraries(xaml_ui xaml_meta)
if(${BUILD_WINDOWS})
    target_link_libraries(xaml_ui Comctl32 Shcore)
elseif(${BUILD_COCOA})
    target_link_libraries(xaml_ui "-framework Cocoa")
elseif(${BUILD_GTK3})
    target_link_libraries(xaml_ui ${GTK3_LIBRARIES})
endif()

install(FILES ${UI_HEADERS} DESTINATION include/xaml/ui)
if(${BUILD_COCOA})
    install(FILES ${UI_COCOA_HEADERS} DESTINATION include/xaml/ui/cocoa)
endif()

add_subdirectory(meta)
