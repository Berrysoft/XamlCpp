project(XamlC CXX)

include_directories(include)

file(GLOB XAMLC_SOURCES "src/*.cpp")

if(${BUILD_WINDOWS})
    find_package(WIL REQUIRED)
    include_directories(${WIL_INCLUDE_DIRS})
elseif(${BUILD_GTK3})
    find_package(GTK3 REQUIRED)
    include_directories(${GTK3_INCLUDE_DIRS})
endif()

find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

include_directories(../meta/include)
include_directories(../ui/include)
include_directories(../xaml/include)

add_executable(xamlc ${XAMLC_SOURCES})
target_link_libraries(xamlc xaml)
