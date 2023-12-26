add_library(algorithms STATIC support_functions.cpp
                              sorts.cpp)

target_include_directories(algorithms PUBLIC ${PROJECT_SOURCE_DIR}/include)