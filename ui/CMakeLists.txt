project(XamlUI CXX)

if(${BUILD_COCOA})
    set(SET_OBJCXX_MANULLY OFF)
    if(CMAKE_VERSION VERSION_LESS 3.16)
        set(SET_OBJCXX_MANULLY ON)
    else()
        enable_language(OBJCXX)
    endif()
endif()

file(GLOB UI_HEADERS "include/xaml/ui/*.hpp")
file(GLOB UI_COCOA_HEADERS "include/xaml/ui/cocoa/*.h")

if(${BUILD_WINDOWS})
    file(GLOB UI_SOURCE "src/win/*.cpp")
elseif(${BUILD_COCOA})
    file(GLOB UI_SOURCE "src/cocoa/*.mm")
elseif(${BUILD_GTK3})
    file(GLOB UI_SOURCE "src/gtk3/*.cpp")
endif()

file(GLOB UI_SHARED_SOURCE "src/shared/*.cpp")

add_library(xaml_ui ${UI_SOURCE} ${UI_SHARED_SOURCE})

target_include_directories(xaml_ui
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${XAML_PRIVATE_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(xaml_ui xaml_meta)
if(${BUILD_COCOA})
    target_link_libraries(xaml_ui "-framework Cocoa")
    if(${SET_OBJCXX_MANULLY})
        target_compile_options(xaml_ui PRIVATE "-xobjective-c++")
    endif()
elseif(${BUILD_GTK3})
    target_link_libraries(xaml_ui ${GTK3_LIBRARIES})
endif()

if(${USE_PCH})
    target_precompile_headers(xaml_ui
        PUBLIC
            <$<BUILD_INTERFACE:xaml/ui/application.hpp>>
            <$<BUILD_INTERFACE:xaml/ui/window.hpp>>
    )
endif()

set_target_properties(xaml_ui PROPERTIES
    VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION}
)

install(FILES ${UI_HEADERS} DESTINATION include/xaml/ui)
if(${BUILD_COCOA})
    install(FILES ${UI_COCOA_HEADERS} DESTINATION include/xaml/ui/cocoa)
endif()
install(TARGETS xaml_ui EXPORT xaml_ui-targets ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(EXPORT xaml_ui-targets DESTINATION lib/cmake/xaml_ui)

configure_file(cmake/xaml_ui-config.cmake.in ${PROJECT_BINARY_DIR}/xaml_ui-config.cmake @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/xaml_ui-config.cmake DESTINATION lib/cmake/xaml_ui)

add_subdirectory(meta)
