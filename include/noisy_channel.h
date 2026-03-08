#ifndef NOISY_CHANNEL_h
#define NOISY_CHANNEL_h

#include "preprocessing.h"
#include "encoder.h" // das ist bestimmt dumm KORREKTUR aber hier kommt SCALE const her





// Signaturen 
void binary_symmetric(std::array<uint64_t,COLS> &codeword);
void gaussian(std::array<uint64_t, COLS> &codeword, std::array<float, COLS*SCALE> &r);

#endif