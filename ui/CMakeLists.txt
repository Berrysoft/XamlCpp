project(XamlUI CXX)

include_directories(include)

include_directories(../meta/include)

file(GLOB UI_HEADERS "include/xaml/ui/*.hpp")

if(WIN32)
if(NOT EXISTS ${VCPKG_ROOT})
message(FATAL_ERROR "Need vcpkg root set as VCPKG_ROOT.")
endif()

include_directories(${VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/include)

add_definitions(-DUNICODE -D_UNICODE -DXAML_UI_WINDOWS -DVC_EXTRALEAN -DWIN32_LEAN_AND_MEAN)

file(GLOB UI_SOURCE "src/win/*.cpp")
endif()

add_library(xaml_ui ${UI_SOURCE})
target_link_libraries(xaml_ui xaml_meta)

install(FILES ${UI_HEADERS} DESTINATION include/xaml/ui)
install(TARGETS xaml_ui EXPORT xaml_ui_targets LIBRARY DESTINATION lib)

file(GLOB TEST_SOURCE "test/*.cpp")
if(WIN32)
add_executable(ui_test WIN32 ${TEST_SOURCE})
else()
add_executable(ui_test ${TEST_SOURCE})
endif()
target_link_libraries(ui_test xaml_ui)
target_link_libraries(ui_test xaml_meta)
