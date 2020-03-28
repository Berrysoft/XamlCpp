project(pugixml VERSION 1.10 LANGUAGES CXX)

add_library(xaml_pugixml ${PROJECT_SOURCE_DIR}/src/pugixml.cpp)
add_library(pugixml ALIAS xaml_pugixml)

if(WIN32)
    target_sources(xaml_pugixml PRIVATE ${PROJECT_SOURCE_DIR}/scripts/pugixml_dll.rc)
endif()

target_include_directories(xaml_pugixml
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_pugixml
        PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:PUGIXML_API=__declspec\(dllexport\)>)
endif()
