add_library(matrix STATIC matrix.cpp)

target_include_directories(matrix PUBLIC ${PROJECT_SOURCE_DIR}/include)