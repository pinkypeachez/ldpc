#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <cstdint>
#include <random>

#include "params.h" // LDPC Parameter



// Signaturen 
void FillParityPart(int8_t (&base)[params::ROWS][params::COLS]);
void FillMessagePart (int8_t (&base)[params::ROWS][params::COLS], std::mt19937 &generator);
bool girthCheck(int8_t (&base)[params::ROWS][params::COLS]);

#endif