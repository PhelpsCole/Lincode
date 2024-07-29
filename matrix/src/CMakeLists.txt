add_library(matrix STATIC matrix.cpp
						  matrix_constructors.cpp
						  matrix_convecters.cpp
						  matrix_functions.cpp)

target_include_directories(matrix PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(algorithms PUBLIC ${CMAKE_SOURCE_DIR}/algorithms/include)

target_link_libraries(matrix algorithms)