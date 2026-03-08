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
    vec.reserve(COLS*SCALE); // durch Dual Diagonal Form wird eig mehr als nötig allociert
    //std::cout << +vec.capacity() << std::endl; //check wie viel allociert wurde
    
    
    for (int row = 0; row < ROWS; row++){
        //std::cout << +h_row << std::endl;
        
        for (int col = 0; col < COLS; col++){
            
            if (h[row][col] >= 0) {
                int8_t index = h[row][col]+col*SCALE;
                std::cout << "Index in Base Matrix: " << +(h[row][col]) << std::endl;
                vec.push_back(index);
                std::cout << +(index) << std::endl;
            }
        }
    }
    
    std::cout << +vec.size() << std::endl;
    



  return 0;
}



