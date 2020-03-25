cmake_minimum_required(VERSION 3.4)
project(pugixml VERSION 1.10 LANGUAGES CXX)

add_library(pugixml STATIC) # Auto selects static or shared based on BUILD_SHARED_LIBS

target_sources(pugixml
  PRIVATE
    ${PROJECT_SOURCE_DIR}/src/pugixml.cpp)

target_include_directories(pugixml
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>)

target_compile_definitions(pugixml
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:PUGIXML_API=__declspec\(dllexport\)>)
