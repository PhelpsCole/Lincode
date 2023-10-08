add_library(codes STATIC rm_code.cpp
                         pqsigRM.cpp
                         linear_code.cpp
                         linear_functions.cpp)

target_include_directories(codes PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(matrix PUBLIC ${CMAKE_SOURCE_DIR}/matrix/include)

target_link_libraries(codes matrix algorithms)