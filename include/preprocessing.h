#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <cstdint>
#include <random>

#include "params.h" // LDPC Parameter



// Signaturen 
void fillMatrix(int8_t (&base)[params::ROWS][params::COLS], std::mt19937 &generator);
bool girthCheck(int8_t (&base)[params::ROWS][params::COLS]);

#endif