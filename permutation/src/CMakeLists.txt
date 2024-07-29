add_library(permutation STATIC permutation.cpp)

target_include_directories(permutation PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(matrix PUBLIC ${CMAKE_SOURCE_DIR}/matrix/include)
target_link_libraries(permutation matrix)