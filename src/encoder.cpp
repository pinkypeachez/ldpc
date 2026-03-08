#include "encoder.h"
#include <iostream>
#include <bitset>
#include <bit>
#include <array>


using namespace std;

void compute_parity(int8_t (&base)[ROWS][COLS], std::array<uint64_t,4> message, int8_t scale, std::array<uint64_t,4> &parity){
    
    for (size_t j = 0; j < ROWS; j++) {
        uint64_t xor_sum = {};
        for (size_t i = 0; i < (COLS-ROWS); i++){
            cout << " Rotate THIS  by " << +(base[j][i]) << ": "<< std::bitset<64>(message[i]) << endl;
            
            uint64_t  rotate = rotr(message[i], base[j][i]);
            xor_sum = xor_sum xor rotate;
        }
        //Accumulator (Dual Diagonal Part of Base Matrix)
        if (j > 0){
            xor_sum = xor_sum xor parity[j-1];
        }
        parity[j] = xor_sum;
    }
    
    
}