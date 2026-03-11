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
            std::cout << "LLR Wert ist: " << llr[index] << std::endl;
            llr[index] < 0 ? local_sign = 1 : local_sign = 0; 
            std::cout << llr[index] << " bekommt Vorzeichen " << local_sign << std::endl;

            check_nodes[cn].global_sign = check_nodes[cn].global_sign xor local_sign;
            

            // finde 2 kleinsten Werte 
            if (std::abs(llr[index]) < check_nodes[cn].min1 and std::abs(llr[index]) < check_nodes[cn].min2){
                if (check_nodes[cn].min1 <= check_nodes[cn].min2){
                    check_nodes[cn].min2 = std::abs(llr[index]);
                } else{
                    check_nodes[cn].min1 = std::abs(llr[index]);
                }
                check_nodes[cn].index_min = index;
            } else if (std::abs(llr[index]) < check_nodes[cn].min1 and std::abs(llr[index]) > check_nodes[cn].min2){
                    check_nodes[cn].min1 = std::abs(llr[index]);
                }
            else if (std::abs(llr[index]) > check_nodes[cn].min1 and std::abs(llr[index]) < check_nodes[cn].min2){
                    check_nodes[cn].min2 = std::abs(llr[index]);

                }
            

/*             if (check_nodes[cn].min1 <= check_nodes[cn].min2) {
                
               
                //dann ist min1 kleinere Wert von beiden --> ersetze min2
                if (std::abs(llr[index]) < check_nodes[cn].min2){
                    check_nodes[cn].min2 = std::abs(llr[index]);
                    
                    //std::cout << std::abs(llr[index]) << " war kleiner als  " << check_nodes[cn].min2 << std::endl;
                } 
    
                }
            else {
                
               
                if (std::abs(llr[index]) < check_nodes[cn].min1){
                    check_nodes[cn].min1 = std::abs(llr[index]);
                    
                }
                
              } */
            /* if (check_nodes[cn].min1 < check_nodes[cn].min2){
                check_nodes[cn].ptr_min = &check_nodes[cn].min1; 
            } else { check_nodes[cn].ptr_min = &check_nodes[cn].min2; } */
             

    }
    std::cout << "XOR Result: " << check_nodes[cn].global_sign <<  std::endl;
    std::cout << "Min1: " << check_nodes[cn].min1 <<  std::endl;
    std::cout << "Min2: " << check_nodes[cn].min2 <<  std::endl;
    int i = check_nodes[cn].index_min;
    std::cout << "GL MIN: " << check_nodes[cn].index_min << " " << llr[i] << std::endl;



    
    }



/*  void CheckNodeUpdate(std::array<float, COLS*SCALE>& llr, std::vector<CheckNode>& check_nodes){  
    // Die Idee ist es, die Nachrichten zu aktualisieren, die von CN an VNs gesendet werden
    // Min * Sign (dabei wird das i-te VN bei der Berechnung ignoriert)

 } */





}