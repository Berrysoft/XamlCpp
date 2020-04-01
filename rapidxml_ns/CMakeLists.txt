project(RapidXmlNs CXX)

add_library(rapidxml_ns INTERFACE)

target_include_directories(rapidxml_ns
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>)
