#ifndef ENCODER_H
#define ENCODER_H

#include <cstdint>
#include <array> 



#include "params.h" // LDPC Parameter






// Signaturen 
void ComputeParity(const int8_t (&base)[params::ROWS][params::COLS], const std::array<uint64_t,(params::COLS-params::ROWS)> &message, std::array<uint64_t,params::ROWS> &parity);

#endif