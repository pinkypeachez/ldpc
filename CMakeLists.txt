cmake_minimum_required(VERSION 3.17)

project(test)

#targets are executable and libraries
#target wie Object
# targets haben Properties, die kann man über Member Function verändern
add_executable(test)

target_sources(test PRIVATE 
    src/main.cpp 
    src/encoder.cpp 
    src/preprocessing.cpp
    src/noisy_channel.cpp
    src/gaussianNoise.cpp
    src/binary_erasure.cpp
    src/decoder.cpp
    src/input.cpp
)
target_include_directories(test PUBLIC include)
target_compile_options(test PUBLIC -O3 -ffast-math -march=native -std=c++20)