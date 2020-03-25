project(pugixml VERSION 1.10 LANGUAGES CXX)

add_library(xaml_pugixml)
add_library(pugixml ALIAS xaml_pugixml)

target_sources(xaml_pugixml
    PRIVATE
        ${PROJECT_SOURCE_DIR}/scripts/pugixml_dll.rc
        ${PROJECT_SOURCE_DIR}/src/pugixml.cpp)

target_include_directories(xaml_pugixml
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>)

target_compile_definitions(xaml_pugixml
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:PUGIXML_API=__declspec\(dllexport\)>)
