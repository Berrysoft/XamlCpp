project(XamlDetector CXX)

file(GLOB XAMLD_SOURCE "src/*.cpp")

add_executable(xamld ${XAMLD_SOURCE})
target_include_directories(xamld PUBLIC include)
target_link_libraries(xamld PUBLIC xaml_cmdline)

find_package(Git)
if(Git_FOUND)
	execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
		RESULT_VARIABLE GIT_RESULT
		OUTPUT_VARIABLE GIT_HASH
	)
	if(GIT_RESULT EQUAL 0)
		string(REPLACE "\n" "" GIT_HASH ${GIT_HASH})
		target_compile_definitions(xamld PRIVATE "XAML_COMMIT_HASH=\"${GIT_HASH}\"")
	endif()
endif()

install(TARGETS xamld RUNTIME DESTINATION bin)
