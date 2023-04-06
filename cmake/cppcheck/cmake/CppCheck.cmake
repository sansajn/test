# CppCheck integration support, report as part of console output during build.
# Dependencies: cppcheck package
function(AddCppCheck target)
	find_program(CPPCHECK_PATH cppcheck REQUIRED)
	set_target_properties(${target}
		PROPERTIES CXX_CPPCHECK
		"${CPPCHECK_PATH};--enable=warning;--inconclusive;--force;--inline-suppr")
endfunction()
