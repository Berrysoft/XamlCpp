project(RapidXmlNs VERSION 1.13 LANGUAGES CXX)

file(GLOB RAPIDXML_HEADERS "include/rapidxml/*.hpp")
file(GLOB RAPIDXML_SOURCE "src/*.cpp")

add_library(xaml_rapidxml ${RAPIDXML_SOURCE})

target_include_directories(xaml_rapidxml
    PUBLIC
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(xaml_rapidxml PUBLIC xaml_global)

if(APPLE)
    target_link_libraries(xaml_rapidxml PUBLIC Boost::container)
endif()

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_rapidxml PRIVATE "RAPIDXML_API=__XAML_EXPORT")
endif()

if(${INSTALL_RAPIDXML_HEADERS})
    install(FILES ${RAPIDXML_HEADERS} DESTINATION include/rapidxml)
endif()
