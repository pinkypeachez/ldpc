// Small Test: Kantenliste

#include <iostream>

constexpr uint8_t ROWS = 4;
constexpr uint8_t COLS = 8;
constexpr uint8_t SCALE = 10;

void FillCNConnections(int (&h) [ROWS][COLS], std::vector <uint8_t>& vec){
    for (int row = 0; row < ROWS; row++){
        
        for (int col = 0; col < COLS; col++){
            
            if (h[row][col] >= 0) {
                int8_t index = h[row][col]+col*SCALE;
                //std::cout << "Index in Base Matrix: " << +(h[row][col]) << std::endl;
                vec.push_back(index);
                //std::cout << +(index) << std::endl;
            }
        }
    }
    
}


int main()
{
    
    // H Matrix
    int h [ROWS][COLS] = {
        4, 3, 7, 6, 0, -1, -1, -1,
        9, 2, 1, 8, 0, 0, -1, -1,
        3, 7, 5, 2, -1, 0, 0, -1,
        1, 3, 4, 9, -1, -1, 0,  0
    };
    
    constexpr uint8_t SCALE = 10;
    
    std::vector <uint8_t> vec;
    vec.reserve(COLS*SCALE); // durch Dual Diagonal Form wird eig mehr als nötig allociert

    FillCNConnections(h,vec);
    
    // ACHTUNG!!! Wegen Dual Diagonal Form hat 1. Check Node -1 weniger Verbindungen als der Rest
    // in Decoder schleife berücksigtigen!

    



  return 0;
}



