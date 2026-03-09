// Kantenliste berechnen + Decoder selbst

#include <iostream>
#include <cstddef> //size_t

#include "decoder.h"

using namespace params;

void FillCNConnections(int8_t (&base) [ROWS][COLS], std::vector <size_t>& nodelist){ //KORREKTUR?? wegen size_t?
    for (size_t row = 0; row < ROWS; row++){
        
        for (size_t col = 0; col < COLS; col++){
            
            if (base[row][col] >= 0) {
                int index = base[row][col]+col*SCALE;
                std::cout << "Index in Base Matrix: " << +(base[row][col]) << std::endl;
                nodelist.push_back(index);
                std::cout << +(index) << std::endl;
            }
        }
    }
    
}