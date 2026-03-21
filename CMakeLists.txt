cmake_minimum_required(VERSION 3.17)

project(ldpc)

#targets are executable and libraries
#target wie Object
# targets haben Properties, die kann man über Member Function verändern
add_executable(ldpc)

target_sources(ldpc PRIVATE 
    src/main.cpp 
    src/encoder.cpp 
    src/preprocessing.cpp
    src/noisy_channel.cpp
    src/gaussianNoise.cpp
    src/binary_erasure.cpp
    src/binary_symmetric.cpp
    src/burst_error.cpp
    src/decoder.cpp
    src/input.cpp
)
target_include_directories(ldpc PUBLIC include)
target_compile_options(ldpc PUBLIC -O3 -ffast-math -march=native -std=c++20)