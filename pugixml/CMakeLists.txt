cmake_minimum_required(VERSION 3.4)
project(pugixml VERSION 1.10 LANGUAGES CXX)

add_library(pugixml)

if(${BUILD_SHARED_LIBS} AND WIN32)
    set_target_properties(pugixml
        PROPERTIES
            PREFIX xaml_
            IMPORT_PREFIX xaml_)
else()
    set_target_properties(pugixml
        PROPERTIES
            PREFIX libxaml_)
endif()

target_sources(pugixml
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/pugixml.cpp)

target_include_directories(pugixml
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>)

target_compile_definitions(pugixml
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:PUGIXML_API=__declspec\(dllexport\)>)
