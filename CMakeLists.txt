cmake_minimum_required (VERSION 4.4.5)
project (coolcalator)
add_executable (coolc)
add_library (maeth)
target_compile_options (maeth PRIVATE -O2 -Werror -Wall -g)
target_sources (maeth
	PRIVATE
		src/lexer.c
		src/trees.c
		src/errors.c
		src/calculator.c
		src/helpers.c
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS
			src
)
target_link_libraries(coolc
	PRIVATE
		maeth
)
target_sources (coolc
	PRIVATE
		src/main.c
)
target_compile_options (coolc PRIVATE -O2 -Werror -Wall -g)
