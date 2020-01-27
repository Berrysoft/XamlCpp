if(WIN32)
	find_path(WIL_INCLUDE_DIRS wil)

	message("-- Found WIL")

	set(WIL_FOUND TRUE)
endif()