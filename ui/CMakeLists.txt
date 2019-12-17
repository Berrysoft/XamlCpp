project(XamlUI)

include_directories(include)

include_directories(../meta/include)

file(GLOB UI_HEADERS "include/xaml/ui/*.hpp")

if(WIN32)
add_definitions(-DUNICODE -D_UNICODE -DXAML_UI_WINDOWS -DVC_EXTRALEAN -DWIN32_LEAN_AND_MEAN)

if(NOT WIL_DIR)
find_package(WIL)
endif()
include_directories(${WIL_DIR})

file(GLOB UI_SOURCE "src/win/*.cpp")
endif()

add_library(xaml_ui ${UI_SOURCE})
target_link_libraries(xaml_ui xaml_meta)

install(FILES ${UI_HEADERS} DESTINATION include/xaml/ui)
install(TARGETS xaml_ui EXPORT xaml_ui_targets LIBRARY DESTINATION lib)

file(GLOB TEST_SOURCE "test/*.cpp")
add_executable(ui_test ${TEST_SOURCE})
target_link_libraries(ui_test xaml_ui)
target_link_libraries(ui_test xaml_meta)
