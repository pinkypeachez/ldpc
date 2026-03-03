#ifndef ENCODER_H
#define ENCODER_H

#include <cstdint>
#include "preprocessing.h"



constexpr uint8_t SCALE = 64;

// Signaturen 
void compute_parity(int8_t (&base)[ROWS][COLS], std::array<uint64_t,4> message, int8_t scale, std::array<uint64_t,4> &parity);


#endif