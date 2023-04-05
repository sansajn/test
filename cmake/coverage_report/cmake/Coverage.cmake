# Function to support coverage report generation (with gcovr) into `coverage` 
# directory in HTML format and cobertura XML report (`coverage.xml`).
# Dependencies: gcovr package
function(AddCoverage target)
	find_program(GCOVR_PATH gcovr REQUIRED)

	add_custom_target(coverage
		COMMENT "Running coverage for ${target}..."

		# run unit tests
		COMMAND $<TARGET_FILE:${target}>
		
		COMMAND mkdir -p coverage
		
		# generate cobertura coverage report
		COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR} --xml-pretty . -o coverage/coverage.xml

		# generate HTML report
		COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR} --html-details -o coverage/index.html

		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	)
endfunction()
