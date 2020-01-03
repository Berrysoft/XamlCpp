project(XamlC CXX)

include_directories(include)

include_directories(../meta/include)
include_directories(../ui/include)
include_directories(../xaml/include)
include_directories(${XAML_PRIVATE_INCLUDE_DIRS})

file(GLOB XAMLC_SOURCES "src/*.cpp")

find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

add_executable(xamlc ${XAMLC_SOURCES})
target_link_libraries(xamlc xaml)

install(TARGETS xamlc DESTINATION bin)
