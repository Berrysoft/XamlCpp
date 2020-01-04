project(XamlC CXX)

file(GLOB XAMLC_SOURCES "src/*.cpp")

add_executable(xamlc ${XAMLC_SOURCES})
target_link_libraries(xamlc xaml)

target_include_directories(xamlc PUBLIC include)

install(TARGETS xamlc DESTINATION bin)
