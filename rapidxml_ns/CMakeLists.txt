project(RapidXmlNs CXX)

file(GLOB RAPIDXML_SOURCE "src/*.cpp")
add_library(rapidxml_ns STATIC ${RAPIDXML_SOURCE})

target_include_directories(rapidxml_ns
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)
