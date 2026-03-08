#ifndef NOISY_CHANNEL_h
#define NOISY_CHANNEL_h

#include "preprocessing.h"
#include "encoder.h" // das ist bestimmt dumm KORREKTUR aber hier kommt SCALE const her





// Signaturen 
void binary_symmetric(std::array<uint64_t,COLS> &codeword);
void GaussianNoise(std::array<uint64_t, COLS> &codeword, std::array<float, COLS*SCALE> &r, float stddev, float a);
float ComputeStdDev(float a, float snr );

#endif