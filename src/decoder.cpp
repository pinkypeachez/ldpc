#include <iostream>
#include <cstddef> //size_t
#include <limits> // um two_mins array zu initialisieren 
#include <cmath> // für

#include "decoder.h"

using namespace params;


// FillCNConnections: 
// Die Indizes der VNs die mit CNs verbunden sind werden ermittelt 
// und in "neighbors"-Liste des CN-Structs gespeichert
void FillCNConnections(int8_t base [ROWS][COLS], std::vector<CheckNode>& check_nodes){ 

    for (size_t row = 0; row < ROWS; row++){     
        for (size_t col = 0; col < COLS; col++){
            int shift = base[row][col];
            //std::cout << "Shift Wert: " << +base[row][col] << std::endl;
            
            if (shift >= 0) { // nur wenn eine Verbindung existiert.. (-1 Eintrag in BaseMatrix steht für "keine Connection")

                for (size_t i = 0 ; i < SCALE; i++) {
                    
                    size_t cn_index = row * SCALE + i; 
                    size_t vn_index = col * SCALE +((i + shift) % 64);
                    
                   // std::cout << "CN Index: " << cn_index << std::endl;
                   // std::cout << "VN Index: " << vn_index << std::endl;


                    check_nodes[cn_index].neighbors.push_back(vn_index);
                    
                    
         /*        // Debugging
                std::cout << "Index in Base Matrix: " << +(base[row][col]) << std::endl;
                std::cout << +(shift) << std::endl;  */
            }
            }
        }
    }
    
}

// Vorbereitung auf CheckNodeUpdate Step eines Decoders
void MinAndSign(std::array<float, COLS*SCALE>& llr, std::vector<CheckNode>& check_nodes){
    bool local_sign = false;

    for (size_t cn = 0; cn < check_nodes.size(); cn++){
        for (size_t n = 0; n < check_nodes[cn].neighbors.size(); n++){
            int index = check_nodes[cn].neighbors[n];

            // berechne globales Vorzeichen
            //std::cout << "LLR Wert ist: " << llr[index] << std::endl;

            // Mappe positive Werte + auf 0, negative - auf 1 
            llr[index] < 0 ? local_sign = 1 : local_sign = 0; 
            //std::cout << llr[index] << " bekommt Vorzeichen " << local_sign << std::endl;

            check_nodes[cn].global_sign = check_nodes[cn].global_sign xor local_sign;
            

            // ------------------- finde 2 kleinsten Werte 
               float llr_magn = std::abs(llr[index]);

               if (llr_magn < check_nodes[cn].min1) {
                //der alte 1.Min-Platz wird zum 2.Platz
                check_nodes[cn].i_min2nd = check_nodes[cn].i_min1st;
                check_nodes[cn].min2 = check_nodes[cn].min1;

                // der LLR Wert wird zum 1.Min-Platz
                check_nodes[cn].i_min1st = index;
                check_nodes[cn].min1 = llr_magn;

               } else if (llr_magn < check_nodes[cn].min2){
                 check_nodes[cn].i_min2nd = index;
                 check_nodes[cn].min2 = llr_magn;
               }

            

             

    }
     // Debugging
    std::cout << "XOR Result: " << check_nodes[cn].global_sign <<  std::endl;
    std::cout << "Min1: " << check_nodes[cn].min1 <<  std::endl;
    std::cout << "Min2: " << check_nodes[cn].min2 <<  std::endl;
    int i = check_nodes[cn].i_min1st;
    int j = check_nodes[cn].i_min2nd;
    std::cout << "GLOBALES MIN1: index " << check_nodes[cn].i_min1st << " Wert: " << llr[i] << std::endl;
    std::cout << "GLOBALES MIN2: index " << check_nodes[cn].i_min2nd << " Wert: " << llr[j] << std::endl;



    
    }
}



    void CheckNodeUpdate(std::array<float, COLS*SCALE>& llr, std::vector<CheckNode>& check_nodes){  
    // Die Idee ist es, die Nachrichten zu aktualisieren, die von CN an VNs gesendet werden
    // Min * Sign (dabei wird das i-te VN bei der Berechnung ignoriert)

     bool sign = 0;
     float smallestValue = 0.0f;

     for (size_t cn = 0; cn < check_nodes.size(); cn++){
        auto& node = check_nodes[cn]; // node = alias für check_nodes[cn]
        std::cout << "\n ========= Check Node " << cn << std::endl;
        std::cout << "Degree: " << node.neighbors.size() << std::endl;

        for (size_t vn = 0; vn < node.neighbors.size(); vn++){

        if (llr[node.neighbors[vn]] < 0){
                sign = 1;
            } else { sign = 0; }
            std::cout << node.global_sign << " xor " << sign << "(" << llr[node.neighbors[vn]] << ") ist ";

            sign = node.global_sign xor sign;
            std::cout << sign << std::endl;


            if (node.neighbors[vn] == node.i_min1st) {
                // Wenn aktueller VN das absolute MIN entspricht, nehmen wir das 2. kleinste Element
                smallestValue = node.min2;
                std::cout << "VN-" <<node.neighbors[vn] << " entspricht MIN am Index " << node.i_min1st << std::endl;
                std::cout << "Als Min wird das 2.kleinste Minima genommen: " << node.min2 << " am Index " << node.i_min2nd << std::endl;
            } else {
                smallestValue = node.min1;
                
            }
        
            sign == 0 ? smallestValue = smallestValue : smallestValue = smallestValue * (-1);

            std::cout << "New LLR " << smallestValue << std::endl;
         //  positive Werte + auf 0, negative - auf 1   

            node.cn2vn[vn]= smallestValue;

        }
     }

    } 





