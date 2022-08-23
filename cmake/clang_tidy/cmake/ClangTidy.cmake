# Clang Tidy integration support, requires *clang-tidy* package installed.
function(AddClangTidy target)
	find_program(CLANG-TIDY_PATH clang-tidy REQUIRED)
	set_target_properties(${target}
		PROPERTIES CXX_CLANG_TIDY
		"${CLANG-TIDY_PATH};-checks=*")
endfunction()
