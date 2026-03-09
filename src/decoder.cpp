#include <iostream>
#include <cstddef> //size_t
#include <limits> // um two_mins array zu initialisieren 
#include <cmath> // für

#include "decoder.h"

using namespace params;

// FillCNConnections: 
// füllt nodelist-Array mit Indizes der VN-Nodes, die mit Check Nodes verbunden sind (Adjazenzliste)
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
    // int n_elements = (COLS-ROWS) + 1; 

    //ENTWEDER HÄSSÖICH DIE ARRAY GRößE ODER LIEBER DYNAMISCHEN VECTOR??? KORREKTUR
    std::array<float, (COLS-ROWS) + 1> llr_fuer_0; // hier werden indizes von LLR Werten gespeichern zu denen der Check Node verbindungen hat
    bool xor_result = 0;
    bool vorzeichen = 0; 

// gleichzeitig rauskriegen zu welchen Nodes dieser Check Node Verbindungen hat + Vorzeichen berechnen
    for (size_t i = 0; i < llr_fuer_0.size(); i++){
        llr_fuer_0[i] = nodelist[i];

        test_llr[i] < 0 ? vorzeichen = 1 : vorzeichen = 0; 
        std::cout << "LLR Wert: " << test_llr[i] << " bekommt Vorzeichen " << vorzeichen << std::endl;

        xor_result = xor_result xor vorzeichen;
        std::cout << "XOR Result: " << xor_result <<  std::endl;

    }

    // MIN SUCHE: 2 kleinste Elemente in je llr_fuer_0 
    // und zwar speichern NICHT den Wert, sondern Index!
    std::array<float, 2> two_mins = { std::numeric_limits<float>::max(), 
                                      std::numeric_limits<float>::max() };
    //two_mins = llr_fuer_0[0];
    int index; //hier wird der Index von dem größeren Element der 2 kleinsten Elemente gespeichert
    for (size_t i = 0; i < llr_fuer_0.size(); i++){
        std::cout << "LLR Wert ist " << test_llr[llr_fuer_0[i]] << std::endl;
        two_mins[0] <= two_mins[1] ? index = 1 : index = 0;
        std::cout << two_mins[index] << " war <= " << two_mins[not(index)] << std::endl;
        std::abs(test_llr[llr_fuer_0[i]]) < two_mins[index] ? two_mins[index] = std::abs(test_llr[llr_fuer_0[i]]) : two_mins[index] = two_mins[index]; // beim nicht erfüllen der bedingung teil ändern KORREKTUR

    }

    for (size_t i = 0; i < two_mins.size(); i++){
        std::cout << two_mins[i] << std::endl;
    }






}