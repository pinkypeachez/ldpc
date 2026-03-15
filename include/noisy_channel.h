#ifndef NOISY_CHANNEL_h
#define NOISY_CHANNEL_h


#include <array> 
#include <vector> 
#include <cstdint>


#include "params.h" // LDPC Parameter



// Signaturen 
void binary_symmetric(std::array<uint64_t,params::COLS>& codeword, float noise_level, std::vector <int>& flipped_bits);
void GaussianNoise(const std::array<uint64_t, params::COLS>& codeword, std::array<float, params::COLS*params::SCALE>& r, const float stddev, const float a);
float ComputeStdDev(const float a, const float snr );

#endif