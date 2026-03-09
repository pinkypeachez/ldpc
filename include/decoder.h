#ifndef DECODER_H
#define DECODER_H

#include "params.h" // LDPC Parameter

#include <vector>   // für std::vector
#include <cstdint>  // für uint8_t






// Signaturen 
void FillCNConnections(int8_t (&base) [params::ROWS][params::COLS], std::vector <size_t>& nodelist);
void VorzeichenCheck(std::array<float, params::SCALE> test_llr, std::vector <size_t> nodelist);


#endif