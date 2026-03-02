#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <cstdint>
#include <random>

constexpr uint8_t ROWS = 4;
constexpr uint8_t COLS = 8;

// Signaturen 
void fillMatrix(int8_t (&base)[ROWS][COLS], std::mt19937 &generator);
bool girthCheck(int8_t (&base)[ROWS][COLS]);

#endif