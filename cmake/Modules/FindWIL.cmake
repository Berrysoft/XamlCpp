if(WIN32)
	find_path(WIL_INCLUDE_DIRS wil)

	message(STATUS "Found WIL")
	set(WIL_FOUND TRUE)

	add_library(wil INTERFACE IMPORTED)
	target_include_directories(wil INTERFACE ${WIL_INCLUDE_DIRS})
endif()