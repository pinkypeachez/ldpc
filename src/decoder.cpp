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
void MinAndSign(const std::array<float, COLS*SCALE>& current_llr, std::vector<CheckNode>& check_nodes){
    
    bool local_sign = false;

    for (size_t cn = 0; cn < check_nodes.size(); cn++){
        auto& node = check_nodes[cn];
        node.reset(); //die minima werden auf float.max() gesetzt, global_sign auf false

        std::cout << " \n ===== Check Node " << cn << std::endl;
         
        for (size_t vn = 0; vn < node.neighbors.size(); vn++){

            int index = node.neighbors[vn];
            std::cout << current_llr[index] << std::endl;
            // CN bekommt die Nachricht AUSGENOMMEN der eigenen Wertes (um sich selbst nicht zu bestätigen)
            float message_to_cn = current_llr[index] - node.cn2vn[vn];

            // berechne globales Vorzeichen
            //std::cout << "LLR Wert ist: " << message_to_cn << std::endl;

            // Mappe positive Werte + auf 0, negative - auf 1 
            message_to_cn < 0 ? local_sign = 1 : local_sign = 0; 
            //std::cout << message_to_cn << " bekommt Vorzeichen " << local_sign << std::endl;

            node.global_sign = node.global_sign xor local_sign;
            

            // ------------------- finde 2 kleinsten Werte 
               float llr_magn = std::abs(message_to_cn);

               if (llr_magn < node.min1) {
                //der alte 1.Min-Platz wird zum 2.Platz
                node.i_min2nd = node.i_min1st;
                node.min2 = node.min1;

                // der LLR Wert wird zum 1.Min-Platz
                node.i_min1st = index;
                node.min1 = llr_magn;

               } else if (llr_magn < node.min2 and llr_magn > node.min1){
                 node.i_min2nd = index;
                 node.min2 = llr_magn;
               }

            

             

    }
     // Debugging
    std::cout << "XOR Result: " << node.global_sign <<  std::endl;
    std::cout << "Min1: " << node.min1 << " index " << node.i_min1st << std::endl;
    std::cout << "Min2: " << node.min2 << " index " << node.i_min2nd <<  std::endl;
    int i = node.i_min1st;
    int j = node.i_min2nd;
    std::cout << "GLOBALES MIN1: index " << node.i_min1st << " Wert: " << current_llr[i] << std::endl;
    std::cout << "GLOBALES MIN2: index " << node.i_min2nd << " Wert: " << current_llr[j] << std::endl;



    
    }
}



    void CheckNodeUpdate(const std::array<float, COLS*SCALE>& current_llr, std::vector<CheckNode>& check_nodes){  
    // Die Idee ist es, die Nachrichten zu aktualisieren, die von CN an VNs gesendet werden
    // Min * Sign (dabei wird das i-te VN bei der Berechnung ignoriert)

     bool sign = 0;
     float smallestValue = 0.0f;

     for (size_t cn = 0; cn < check_nodes.size(); cn++){
        auto& node = check_nodes[cn]; // node = alias für check_nodes[cn]
        //std::cout << "\n ========= Check Node " << cn << std::endl;
        //std::cout << "Degree: " << node.neighbors.size() << std::endl;

        for (size_t vn = 0; vn < node.neighbors.size(); vn++){

        if (current_llr[node.neighbors[vn]] < 0){
                sign = 1;
            } else { sign = 0; }
            //std::cout << node.global_sign << " xor " << sign << "(" << current_llr[node.neighbors[vn]] << ") ist ";

            sign = node.global_sign xor sign;
            //std::cout << sign << std::endl;


            if (node.neighbors[vn] == node.i_min1st) {
                // Wenn aktueller VN das absolute MIN entspricht, nehmen wir das 2. kleinste Element
                smallestValue = node.min2;
              //  std::cout << "VN-" <<node.neighbors[vn] << " entspricht MIN am Index " << node.i_min1st << std::endl;
               // std::cout << "Als Min wird das 2.kleinste Minima genommen: " << node.min2 << " am Index " << node.i_min2nd << std::endl;
            } else {
                smallestValue = node.min1;
                
            }
        
            sign == 0 ? smallestValue = smallestValue : smallestValue = smallestValue * (-1);

           // std::cout << "New LLR " << smallestValue << std::endl;
         //  positive Werte + auf 0, negative - auf 1   

            node.cn2vn[vn]= smallestValue;

        }
     }

    } 

void VarNodeUpdate(const std::array<float, params::COLS*params::SCALE>& llr, std::array<float, COLS*SCALE>& current_llr, const std::vector<CheckNode>& check_nodes){
  std::array<float, params::COLS*params::SCALE> vn_sum = {};

// die kompakte Form des cn2vn auf die volle Größe aufblasen
// alle LLR Werte (pro VN) werden aufsummiert
  for (size_t cn = 0; cn < check_nodes.size(); cn++){
        auto& node = check_nodes[cn];
       for (size_t n = 0; n < node.neighbors.size(); n++){
        size_t i_vn = node.neighbors[n];
        vn_sum[i_vn] += node.cn2vn[n];

       } 
    }

  for (size_t vn = 0; vn < llr.size(); vn++) {
    current_llr[vn] = llr[vn] + vn_sum[vn];
  }
}

void HardDecision(const std::array<float, params::COLS*params::SCALE>& current_llr, std::array<int, params::COLS*params::SCALE>& calc_codeword){
    // ohne & weil ich die LLR Werte nicht verändern will

    for (size_t i = 0; i < current_llr.size(); i++){
        current_llr[i] >= 0? calc_codeword[i] = 0 : calc_codeword[i] = 1;
        //zur erinnerung:  positive Werte + auf 0, negative - auf 1 
    }

}





