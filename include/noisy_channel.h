#pragma once


#include <array> 
#include <vector> 
#include <cstdint>


#include "params.h" // LDPC Parameter


// Interface (Template für Noisy Channel)
class INoisyChannel {
  public:
    virtual ~INoisyChannel() = default; // destruktor ???????
    // für alle NoiseModels gleich:  LLRs müssen aus dem codewort berechnet werden
    virtual void applyNoise(const std::array<uint64_t, params::COLS>& codeword, std::array<float, params::COLS*params::SCALE>& llr) = 0;
};



class BinaryErasure : public INoisyChannel {
    public:
};


// Signaturen 
void binary_symmetric(std::array<uint64_t,params::COLS>& codeword, float noise_level, std::vector <int>& flipped_bits);
void BinaryErasure(const float erasureProb, const std::vector <int>& erasureIndex);
//void GaussianNoise(const std::array<uint64_t, params::COLS>& codeword, std::array<float, params::COLS*params::SCALE>& r, const float stddev, const float a);
//float ComputeStdDev(const float a, const float snr );
