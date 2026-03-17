#pragma once

#include "params.h" // LDPC Parameter

#include <vector>   // für std::vector
#include <cstdint>  // für uint8_t


struct CheckNode {

// speichertechnisch nicht die smarteste Lösung aber es passt
    std::vector<size_t> neighbors;   // Indizes der VNs, die mit diesem CN verbunden sind
    std::vector<float>  cn2vn; // Nachrichten die an die VNs geschickt werden

    CheckNode() {
        neighbors.reserve(params::COLS); 
        //cn2vn.reserve(params::COLS);
        cn2vn.assign(params::COLS * params::SCALE, 0.0f); // DIE LÖSUNG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    // KORREKTUR ??
    // eigentlich müssen min1 und min2 nicht Teil des Structs sein, weil sie nur
    // in der Schleife und nirgendwo sonst benutzt werden
    float min1 = std::numeric_limits<float>::max();
    float min2 = std::numeric_limits<float>::max();
    bool global_sign = false;

    // Indizes der 2 kleinsten Werte KORREKTUR? bzw wenn ich schon Minima Werte speichere,
    //brauche ich nur das Index von dem allerkleinsten Element
    int i_min1 = -1;
    int i_min2 = -1; 



    // vor jeder iteration
    void reset() {
        min1 = std::numeric_limits<float>::max();
        min2 = std::numeric_limits<float>::max();
        global_sign = false;
        i_min1 = -1;
        i_min2 = -1;
   


    }
};



// Signaturen 
void FillCNConnections(const int8_t base [params::ROWS][params::COLS], std::vector<CheckNode>& check_nodes);
void MinAndSign(const std::array<float, params::COLS*params::SCALE>& current_llr, std::vector<CheckNode>& check_nodes);
void CheckNodeUpdate(const std::array<float, params::COLS*params::SCALE>& current_llr, std::vector<CheckNode>& check_nodes);
void VarNodeUpdate(const std::array<float, params::COLS*params::SCALE>& llr, std::array<float, params::COLS*params::SCALE>& current_llr, const std::vector<CheckNode>& check_nodes);
void HardDecision(const std::array<float, params::COLS*params::SCALE>& current_llr, std::bitset<params::COLS*params::SCALE>& calc_codeword);
void CalcSyndrom();
