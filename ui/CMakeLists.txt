project(XamlUI CXX)

if(${BUILD_COCOA})
    if(CMAKE_VERSION VERSION_LESS 3.16)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")
    else()
        enable_language(OBJCXX)
    endif()
endif()

include_directories(include)

include_directories(../meta/include)

file(GLOB UI_HEADERS "include/xaml/ui/*.hpp")

if(${BUILD_WINDOWS})
    find_package(WIL REQUIRED)
    include_directories(${WIL_INCLUDE_DIRS})

    file(GLOB UI_SOURCE "src/win/*.cpp")
elseif(${BUILD_COCOA})
    file(GLOB UI_SOURCE "src/cocoa/*.mm")
elseif(${BUILD_GTK3})
    find_package(GTK3 REQUIRED)
    include_directories(${GTK3_INCLUDE_DIRS})

    file(GLOB UI_SOURCE "src/gtk3/*.cpp")
endif()

file(GLOB UI_SHARED_SOURCE "src/shared/*.cpp")

add_library(xaml_ui ${UI_SOURCE} ${UI_SHARED_SOURCE})
target_link_libraries(xaml_ui xaml_meta)
if(${BUILD_WINDOWS})
    target_link_libraries(xaml_ui gdiplus)
elseif(${BUILD_COCOA})
    if(${USE_GNUSTEP})
        target_link_libraries(xaml_ui gnustep-gui gnustep-base objc)
    else()
        target_link_libraries(xaml_ui "-framework Cocoa")
    endif()
elseif(${BUILD_GTK3})
    target_link_libraries(xaml_ui ${GTK3_LIBRARIES})
endif()

install(FILES ${UI_HEADERS} DESTINATION include/xaml/ui)
install(TARGETS xaml_ui EXPORT xaml_ui_targets LIBRARY DESTINATION lib)

file(GLOB TEST_SOURCE "test/*.cpp")
if(${BUILD_WINDOWS})
    add_executable(ui_test WIN32 ${TEST_SOURCE})
else()
    add_executable(ui_test ${TEST_SOURCE})
endif()
target_link_libraries(ui_test xaml_ui)
target_link_libraries(ui_test xaml_meta)
