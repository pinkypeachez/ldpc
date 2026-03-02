// Die Base Matrix wird erstellt, Girth-4 Check gemacht
//

#include <iostream>
#include <random>

#include "preprocessing.h"
#include "encoder.h"

using namespace std;


int main() {
    // ------------- PREPROCESSING STAGE
    // Base Matrix wird erstellt, Girth-4 Check durchgeführt

    //  --- Initialisierung des Zufallszahlgenerators
    random_device rd;
    uint seed = rd();
    std::mt19937 generator {seed};
    cout << " Seed für mt19937 Generator ist " << seed << endl;

    cout << " Matrix wird gefüllt " << endl;

    int8_t base[ROWS][COLS] = {};

    bool girthOk = false;

    while (!girthOk) {
        fillMatrix(base, generator);
        girthOk = girthCheck(base);
    }
    
    std::cout << "Base Matrix mit Girth-6 gefunden!" << std::endl;


    // --- ENCODER STAGE
    std::array<uint64_t,ROWS> message= {
        0b0110110001010100011010100111001001101101011100000110110010110010,
        0b0110010001010001011010100111010001101101011100000110110011010010,
        0b0100111001010000011010100111000011101111011101100110110011110010,
        0b0110110001010000011010100111001001101101011100000110110001110010
    };
    std::array<uint64_t,ROWS> parity= {};
    int8_t scale = 64;


    //compute_parity(base, message, scale, parity);
    //cout << std::bitset<64>(parity) << endl;

  
    
/*     cout << " Ausgabe Matrix " << endl;
    for (uint8_t i = 0; i < 4; i++){
        for (uint8_t j = 0; j < 8; j++) {
            std::cout << +base[i][j] << std::endl;
            
        }
    } */



    

    
    
    return 0;
}
