#ifndef ENCODER_H
#define ENCODER_H

#include <cstdint>
#include <array> 



#include "params.h" // LDPC Parameter






// Signaturen 
void compute_parity(int8_t (&base)[params::ROWS][params::COLS], std::array<uint64_t,4> message, std::array<uint64_t,4> &parity);


#endif