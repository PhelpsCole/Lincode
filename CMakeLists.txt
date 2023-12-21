cmake_minimum_required(VERSION 3.10.2)
project(lincode)

set(CMAKE_CXX_STANDARD 17)


add_compile_options( -W  -Wall      -pedantic       -fstack-protector-all
                     -Wextra        -Wfloat-equal
                     -Wcast-align   -Wpointer-arith -Wformat-security
                     -Wformat       -Wwrite-strings -Wcast-align
                     -Wno-long-long -Wunused        -Wcast-qual)

add_subdirectory(algorithms)
add_subdirectory(matrix)
add_subdirectory(codes)
add_subdirectory(support_splitting)
add_subdirectory(code_attacks)