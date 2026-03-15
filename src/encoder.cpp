#include "encoder.h"
#include <iostream>
#include <bitset>
#include <bit>
#include <array>

using namespace params;

void ComputeParity (const int8_t (&base)[ROWS][COLS], const std::array<uint64_t, (COLS-ROWS)> &message, std::array<uint64_t, ROWS> &parity)
{
    
    for (size_t row = 0; row < ROWS; row++) //für jeder CN:
    {
        uint64_t xor_sum = 0;
        for (size_t i = 0; i < (COLS - ROWS); i++)
        {
            if (base[row][i] >= 0){
            uint64_t rotate = std::rotr(message[i], base[row][i]);
            std::cout << std::bitset<64>(message[i]) << " " << std::bitset<64>(rotate) << " um " << +base[row][i]<< std::endl;
            xor_sum ^= rotate;   // xor_sum = xor_sum xor rotate;  
            }  
        }

        // Accumulator (wegen Dual Diagonal Form von Base Matrix)
        if (row > 0) //  0.Zeile wird übersprungen (nicht akkumuliert)
        {
            xor_sum ^= parity[row - 1];  // xor_sum xor parity[row - 1];
        }
        parity[row] = xor_sum;
    }
}


// Check ob Encoder funktioniert
bool CheckCodeword(const int8_t (&base)[ROWS][COLS], const std::array<uint64_t, COLS>& codeword){
    for (size_t row = 0; row < ROWS; row++) {

        uint64_t acc = 0;

        for (size_t col = 0; col < COLS; col++) {

            if (base[row][col] >= 0) {

                uint64_t rotated =
                    std::rotr(codeword[col], base[row][col]);

                acc ^= rotated;
            }
        }

        if (acc != 0) {
            std::cout << "Problem bei Parity Check in Row: " << row << std::endl;
            return false;
        }
    }

    return true;
}
