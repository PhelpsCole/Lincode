add_library(support_splitting STATIC ssa_structure.cpp
                                     n-ssa_structure.cpp
                                     invariants.cpp
                                     ssa_test.cpp)

target_include_directories(support_splitting PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(matrix PUBLIC ${CMAKE_SOURCE_DIR}/matrix/include)
target_include_directories(codes PUBLIC ${CMAKE_SOURCE_DIR}/codes/include)

target_link_libraries(support_splitting matrix codes)