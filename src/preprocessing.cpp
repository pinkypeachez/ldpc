#include "preprocessing.h"
#include <random>
#include <iostream>


using namespace std;

void fillMatrix (int8_t (&base)[ROWS][COLS], std::mt19937 &generator){
    std::uniform_int_distribution<int> distribution(0,63);


  // HAT WAS MIT CODE RATE ZU TUN ABER ERSTMAL HARDCODED!!!!!!!!!!!
    // für DUAL DIAGONAL FORM FÜLLEN
    for (uint8_t i = 0; i < ROWS; i++){
        for (uint8_t j = 4; j < COLS; j++) {
            ((i == (j-4)) or (i == (j - 4 + 1))) ? base[i][j] = 0 : base[i][j] = -1;
        }
    }
    
    //--- "A" TEIL DER BASE MATRIX MIT ZUFÄLLIGEN WERTEN FÜLLEN
    for (uint8_t i = 0; i < 4; i++){
        for (uint8_t j = 0; j < 4; j++) {
            base[i][j] = distribution(generator);  
        }
    }

    cout << " Ausgabe Matrix " << endl;
    for (uint8_t i = 0; i < 4; i++){
        cout << " " << endl;
        for (uint8_t j = 0; j < 8; j++) {
            std::cout << +base[i][j] << " ";
            
        }
    }  
};

bool girthCheck ( int8_t (&base) [ROWS][COLS]){
   //std::cout << " Girth-4 Check beginnt... "<< std::endl;
    //cout << +COLS << +ROWS << endl;

    for (uint8_t step_c = 1; step_c < COLS; step_c++) {
      
      for (uint8_t step_r = 1; step_r < ROWS; step_r++) {
        for ( uint8_t i = 0; i < ROWS - step_r; i++ ){
            for (uint8_t j = 0; j < COLS - step_c; j++){
              if (base[i][j] != -1 and base[i+step_r][j] != -1 and base[i][j+step_c] != -1 and base[i+step_r][j+step_c] != -1){
                // das mit Modulo 64 dokumentieren, z:B wieso denn "+64"?

                // ----- Modulo 64 via Bit-Maske !!!!!!!!
                if (((base[i][j] - base[i+step_r][j]) + (base[i+step_r][j+step_c]-base[i][j+step_c])+64) % 64 == 0) {
                    cout << " Check nicht bestanden. Girth-4: " << +base[i][j] << " " << +base[i+step_r][j] << " " << +base[i+step_r][j+step_c] << " " << +base[i][j+step_c] << endl;
                    return 0;
                }
            }
          }
        }
      }
    }


    return 1;

};