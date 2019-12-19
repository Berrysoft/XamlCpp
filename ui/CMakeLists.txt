if(${BUILD_COCOA})
    project(XamlUI OBJCXX)
else()
    project(XamlUI CXX)
endif()

include_directories(include)

include_directories(../meta/include)

file(GLOB UI_HEADERS "include/xaml/ui/*.hpp")

if(${BUILD_WINDOWS})
    if(NOT EXISTS ${VCPKG_ROOT})
        message(FATAL_ERROR "Need vcpkg root set as VCPKG_ROOT.")
    endif()

    include_directories(${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include)

    add_definitions(-DUNICODE -D_UNICODE -DXAML_UI_WINDOWS -DVC_EXTRALEAN -DWIN32_LEAN_AND_MEAN)

    file(GLOB UI_SOURCE "src/win/*.cpp")
elseif(${BUILD_COCOA})
    add_definitions(-DXAML_UI_COCOA)

    file(GLOB UI_SOURCE "src/cocoa/*.mm")
elseif(${BUILD_GTK3})
    add_definitions(-DXAML_UI_GTK3)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

    include_directories(${GTK3_INCLUDE_DIRS})
    link_directories(${GTK3_LIBRARY_DIRS})

    add_definitions(${GTK3_CFLAGS_OTHER})

    file(GLOB UI_SOURCE "src/gtk3/*.cpp")
endif()

add_library(xaml_ui ${UI_SOURCE})
target_link_libraries(xaml_ui xaml_meta)
if(${BUILD_COCOA})
    target_link_libraries(xaml_ui "-framework Cocoa")
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
