#include "preprocessing.h"
#include <random>
#include <iostream>


using namespace std;
using namespace params;

void createBaseMatrix(int8_t (&base) [ROWS][COLS], std::mt19937_64& generator){
    std::cout << "[INFO] BASE MATRIX INITIALISATION" << std::endl;
    FillParityPart(base); //Dual Diagonal Form
    FillMessagePart(base,generator); // CN Degree ist gleich, ausgenommen 0.Zeile (bzw.0-SCALE Zeilen) - Quasi Irregular LDPC
    
    bool girthOk = false;
    size_t count = 0;
    while (!girthOk) {
        count++;
   //     std::cout << "[DEBUG] Girth-4 Check NICHT bestanden, neuer Versuch..." << std::endl;
        FillMessagePart(base, generator);
        girthOk = girthCheck(base);
    }
    std::cout << "[INFO] Girth-4 Check completed at " << count << " attempt." << std::endl;

    for (size_t i = 0; i < ROWS; i++){
        std::cout << " " << std::endl;
        for (size_t j = 0; j < COLS; j++) {
            std::cout << +base[i][j] << " ";     
        }
    } 
}

void FillParityPart (int8_t (&base)[ROWS][COLS]){

// Rechter Teil der Base Matrix (Dual Diagonal Form) wird gefüllt
    for (size_t row = 0; row < ROWS; row++){
      for (size_t col = ROWS; col < COLS; col++) {

        if (row == (col - ROWS) || row == (col - ROWS + 1))
            base[row][col] = 0;
        else
            base[row][col] = -1;
    }
}
    
};

void FillMessagePart (int8_t (&base)[ROWS][COLS], std::mt19937_64 &generator){
  // Linker Teil der Base Matrix (message bits) wird mit zufälligen Shift Werten gefüllt
  // Es werden Shift Werte zwischen 0 und 63 (für SCALE = 64) verteilt
    std::uniform_int_distribution<int> distribution(0,SCALE-1);

    for (size_t i = 0; i < ROWS; i++){
        for (size_t j = 0; j < COLS-ROWS; j++) {
            base[i][j] = distribution(generator);  
        }
    }
};


bool girthCheck ( int8_t (&base) [ROWS][COLS]){
   //std::cout << " Girth-4 Check beginnt... "<< std::endl;
    //cout << +COLS << +ROWS << endl;

    for (size_t step_c = 1; step_c < COLS; step_c++) {
      
      for (size_t step_r = 1; step_r < ROWS; step_r++) {
        for ( size_t i = 0; i < ROWS - step_r; i++ ){
            for (size_t j = 0; j < COLS - step_c; j++){
              if (base[i][j] != -1 and base[i+step_r][j] != -1 and base[i][j+step_c] != -1 and base[i+step_r][j+step_c] != -1){
                // das mit Modulo 64 dokumentieren, z:B wieso denn "+64"?

                // ----- Modulo 64 via Bit-Maske !!!!!!!!
                if (((base[i][j] - base[i+step_r][j]) + (base[i+step_r][j+step_c]-base[i][j+step_c])+64) % 64 == 0) {
                    cout << "Failed at Girth-4: " << +base[i][j] << " " << +base[i+step_r][j] << " " << +base[i+step_r][j+step_c] << " " << +base[i][j+step_c] << endl;
                    return false;
                }
            }
          }
        }
      }
    }


    return true;

};