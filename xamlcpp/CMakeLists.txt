project(XamlCompilerCpp CXX)

file(GLOB XAML_CPP_SOURCE "src/*.cpp")

add_executable(xamlcpp ${XAML_CPP_SOURCE})
target_link_libraries(xamlcpp PUBLIC xaml_compiler xaml_cmdline)

find_package(Git)
if(Git_FOUND)
	execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
		RESULT_VARIABLE GIT_RESULT
		OUTPUT_VARIABLE GIT_HASH
	)
	if(GIT_RESULT EQUAL 0)
		string(REPLACE "\n" "" GIT_HASH ${GIT_HASH})
		target_compile_definitions(xamlcpp PRIVATE "XAML_COMMIT_HASH=\"${GIT_HASH}\"")
	endif()
endif()

install(TARGETS xamlcpp RUNTIME DESTINATION bin)
