#ifndef DECODER_H
#define DECODER_H

#include "params.h" // LDPC Parameter

#include <vector>   // für std::vector
#include <cstdint>  // für uint8_t


struct CheckNode {
    float min1 = std::numeric_limits<float>::max();
    float min2 = std::numeric_limits<float>::max();
    bool global_sign = false;

    //pointer auf den kleinsten der 2 Minima
    // bzw wäre es nur den Index abzuspeichern smarter
    int index_min = -1; 


    // Init: drin werden Indizes der VNs gespeichert, 
    // die mit diesem CN verbunden sind
    std::vector<size_t> neighbors;

    // vor jeder iteration
    void reset() {
        min1 = std::numeric_limits<float>::max();
        min2 = std::numeric_limits<float>::max();
        global_sign = false;
        index_min = -1;

    }
};



// Signaturen 
void FillCNConnections(int8_t base [params::ROWS][params::COLS], std::vector<CheckNode>& check_nodes);
void MinAndSign(std::array<float, params::COLS*params::SCALE>& llr, std::vector<CheckNode>& check_nodes);
//void CheckNodeUpdate(std::array<float, params::COLS*params::SCALE>& llr, std::vector<CheckNode>& check_nodes);

#endif