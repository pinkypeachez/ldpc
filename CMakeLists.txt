cmake_minimum_required(VERSION 3.17)

project(test)

#targets are executable and libraries
#target wie Object
# targets haben Properties, die kann man über Member Function verändern
add_executable(test)
target_sources(test PUBLIC main.cpp encoder.cpp preprocessing.cpp)
target_compile_options(test PUBLIC -Ofast -std=c++20)