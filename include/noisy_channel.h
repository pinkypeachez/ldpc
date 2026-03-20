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
    virtual void applyNoise(const std::array<float, params::COLS*params::SCALE>& t,std::array<float, params::COLS*params::SCALE>& llr) = 0;
};



void MapToSignalVector(const std::array<uint64_t, params::COLS> &codeword, 
              std::array<float, params::COLS*params::SCALE>& llr, const float a);