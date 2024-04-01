add_library(code_attacks STATIC chizhov_borodin.cpp
                                minder_shokrollahi.cpp
                                modRM_remover.cpp
                                modRM_attack.cpp
                                concatenateRM_attack.cpp
                                separate_RM_blocks.cpp
                                hadPowerCounter.cpp
                                hadPowerCounterTwice.cpp
                                hadPowerCounterSelectors.cpp
                                separateConcatenatedCodes.cpp)

target_include_directories(code_attacks PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(matrix PUBLIC ${CMAKE_SOURCE_DIR}/matrix/include)
target_include_directories(codes PUBLIC ${CMAKE_SOURCE_DIR}/codes/include)
target_include_directories(algorithms PUBLIC ${CMAKE_SOURCE_DIR}/algorithms/include)
target_include_directories(support_splitting PUBLIC ${CMAKE_SOURCE_DIR}/algorithms/include)

target_link_libraries(code_attacks matrix codes algorithms support_splitting)