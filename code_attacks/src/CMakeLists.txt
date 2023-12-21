add_library(code_attacks STATIC code_attacks.cpp)

target_include_directories(code_attacks PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(matrix PUBLIC ${CMAKE_SOURCE_DIR}/matrix/include)
target_include_directories(codes PUBLIC ${CMAKE_SOURCE_DIR}/codes/include)

target_link_libraries(code_attacks matrix codes)