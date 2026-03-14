#include "encoder.h"
#include <iostream>
#include <bitset>
#include <bit>
#include <array>

using namespace params;

void ComputeParity (const int8_t (&base)[ROWS][COLS], const std::array<uint64_t, 4> &message, std::array<uint64_t, 4> &parity)
{
    uint64_t xor_sum;
    for (size_t j = 0; j < ROWS; j++)
    {
        xor_sum = {};
        for (size_t i = 0; i < (COLS - ROWS); i++)
        {
            uint64_t rotate = std::rotr(message[i], base[j][i]);
            xor_sum ^= rotate;   // xor_sum = xor_sum xor rotate;    
        }

        // Accumulator (Dual Diagonal Part of Base Matrix)
        if (j > 0) //  0.Zeile wird übersprungen (nicht akkumuliert)
        {
            xor_sum ^= parity[j - 1];  // xor_sum xor parity[j - 1];
        }
        parity[j] = xor_sum;
    }
}

