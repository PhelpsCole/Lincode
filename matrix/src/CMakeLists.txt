add_library(matrix STATIC matrix.cpp
						  matrix_constructors.cpp
						  matrix_convecters.cpp
						  matrix_functions.cpp)

target_include_directories(matrix PUBLIC ${PROJECT_SOURCE_DIR}/include)