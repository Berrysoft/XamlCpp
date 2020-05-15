project(XamlCmdlineHelper CXX)

file(GLOB XAML_CMDLINE_HELPER_SOURCE "src/*.cpp")
add_library(xaml_cmdline_helper ${XAML_CMDLINE_HELPER_SOURCE})

target_include_directories(xaml_cmdline_helper
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

if(${BUILD_SHARED_LIBS})
    target_compile_definitions(xaml_cmdline_helper PRIVATE "XAML_CMDLINE_HELPER_API=__XAML_EXPORT")
endif()

find_package(Git)
if(Git_FOUND)
	execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
		RESULT_VARIABLE GIT_RESULT
		OUTPUT_VARIABLE GIT_HASH
	)
	if(GIT_RESULT EQUAL 0)
		string(REPLACE "\n" "" GIT_HASH ${GIT_HASH})
		target_compile_definitions(xaml_cmdline_helper PRIVATE "XAML_COMMIT_HASH=\"${GIT_HASH}\"")
	endif()
endif()

target_link_libraries(xaml_cmdline_helper PUBLIC xaml_cmdline)

install(
	TARGETS xaml_cmdline_helper
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
