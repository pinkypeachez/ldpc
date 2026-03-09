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
                nodelist.push_back(index);
                /* Debugging
                std::cout << "Index in Base Matrix: " << +(base[row][col]) << std::endl;
                std::cout << +(index) << std::endl; */
            }
        }
    }
    
}

//TODOs: Vorzeichen Check + 2 kleinste Einträge finden
// Vorzeichen Check: + auf 0, - auf 1 mappen, xor'ren
void VorzeichenCheck(std::array<float, SCALE> test_llr, std::vector <size_t> nodelist){
    std::cout << "Hallo aus func bitches" << std::endl;
    
    // PLAN:
    // 1. VORZEICHEN CHECK:
    //Nimm nur die Werte aus LLR Liste die mit diesem CheckNode verbunden sind
    // Mappe positive Werte + auf 0, negative - auf 1 
    // Xor're sie alle um Vorzeichen rauszukriegen

    // 2. MIN SUCHE (2 kleinste Einträge) einfach manuell

    // achso das 0.Element hat -1 Degree im Vegleich zu anderen Check Nodes!!!!
    // 0.Element: Anzahl Message Bits (COLS-ROWS) + 1 
    // ab 1.Element: Anzahl Message Bits (COLS-ROWS) + 2 


}