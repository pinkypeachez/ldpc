cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20) #wegen <bit> ist c++ 20 wichtig
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ldpc)

add_executable(ldpc)

target_sources(ldpc PRIVATE 
    src/main.cpp 
    src/argument_parser.cpp
    src/input_string.cpp
    src/preprocessing.cpp
    src/encoder.cpp 
    src/noisy_channel/noisy_channel.cpp
    src/noisy_channel/gaussian_noise.cpp
    src/noisy_channel/binary_erasure.cpp
    src/noisy_channel/binary_symmetric.cpp
    src/noisy_channel/burst_error.cpp
    src/decoder.cpp
)
target_include_directories(ldpc PUBLIC include)
target_compile_options(ldpc PUBLIC -O3 -ffast-math -march=native -std=c++20)