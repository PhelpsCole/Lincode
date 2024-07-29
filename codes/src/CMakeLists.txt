add_library(codes STATIC rm_code.cpp
                         pqsigRM.cpp
                         encoder.cpp
                         linear_code.cpp
                         linear_functions.cpp)

target_include_directories(codes PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(matrix PUBLIC ${CMAKE_SOURCE_DIR}/matrix/include)
target_include_directories(algorithms PUBLIC ${CMAKE_SOURCE_DIR}/algorithms/include)
target_include_directories(permutation PUBLIC ${CMAKE_SOURCE_DIR}/permutation/include)

target_link_libraries(codes matrix algorithms permutation)