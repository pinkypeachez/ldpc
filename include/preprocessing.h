#pragma once

#include <cstdint>
#include <random>

#include "params.h" // LDPC Parameter



// Signaturen 
void createBaseMatrix(int8_t (&base) [params::ROWS][params::COLS], std::mt19937_64& generator);
void FillParityPart(int8_t (&base)[params::ROWS][params::COLS]);
void FillMessagePart (int8_t (&base)[params::ROWS][params::COLS], std::mt19937_64 &generator);
bool girthCheck(int8_t (&base)[params::ROWS][params::COLS]);

