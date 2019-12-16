project(XamlUI)

include_directories(include)

file(GLOB UI_HEADERS "include/xaml/ui/*.hpp")

install(FILES ${UI_HEADERS} DESTINATION include/xaml/ui)

file(GLOB TEST_SOURCE "test/*.cpp")
add_executable(ui_test ${TEST_SOURCE})
