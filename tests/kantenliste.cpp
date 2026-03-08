// Small Test: Kantenliste

#include <iostream>
#include <iostream>


int main()
{
    constexpr uint8_t ROWS = 4;
    constexpr uint8_t COLS = 8;
    
    // H Matrix
    int h [ROWS][COLS] = {
        4, 3, 7, 6, 0, -1, -1, -1,
        9, 2, 1, 8, 0, 0, -1, -1,
        3, 7, 5, 2, -1, 0, 0, -1,
        1, 3, 4, 9, -1, -1, 0,  0
    };
    
    constexpr uint8_t SCALE = 10;
    
    std::vector <uint8_t> vec;
    vec.reserve(COLS*SCALE); //IST DAS SINNVOL??????
    std::cout << +vec.capacity() << std::endl; //check wie viel allociert wurde
    
    
    uint8_t h_row = 1/SCALE; // ansonsten statt 1 counter für rows
    //std::cout << +h_row << std::endl;
    
    for (int col = 0; col < COLS; col++){

        if (h[h_row][col] >= 0) {
            int8_t index = h[h_row][col]+col*SCALE;
            std::cout << +(h[h_row][col]) << std::endl;
            vec.push_back(index);
            std::cout << +(index) << std::endl;
        }
    }
    
    



  return 0;
}



