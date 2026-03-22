#include <iostream>
#include <random>
#include <cmath> // fürs aufrunden
#include <string> // um teile vom string zu nehmen

#include "argument_parser.h"
#include "preprocessing.h"
#include "encoder.h"
#include "noisy_channel.h"
#include "gaussianNoise.h"
#include "binary_erasure.h"
#include "binary_symmetric.h"
#include "burst_error.h"
#include "decoder.h"
#include "params.h"
#include "input_string.h"

using namespace params;


int main(int argc, char* argv[]) {

// Parsen der Konsolenargumente 
    ArgumentParser arguments(argc, argv);
    arguments.parse();

// Input-String (oder Default-String) wird in Chunks aufgeteilt
    MessageDispatcher m;
    m.dispatch(arguments.getInput());


// Preprocessing: BaseMatrix wird erstellt   
    std::random_device rd;
    uint seed = rd();
    std::mt19937 generator {seed};

    int8_t base[ROWS][COLS] = {};

    createBaseMatrix(base, generator);

// ============ START: "Encoder -> Noisy Channel -> Decoder" Kette in range of "numberOfChunks" ============ 

    for (size_t chunk = 0; chunk < m.numberOfChunks; chunk++){
        std::array<uint64_t,COLS-ROWS> message = m.chunks[chunk];


//Encoder
        std::array<uint64_t,ROWS> parity= {};
        ComputeParity(base, message, parity);

//Message Bits & Parity Bits werden zusammengefügt
        std::array<uint64_t, COLS> codeword;
        std::copy(message.begin(), message.end(), codeword.begin());
        std::copy(parity.begin(), parity.end(), codeword.begin() + (COLS-ROWS)); 


//Noisy Channel

     // 1: Map to a Signal Vector (0 wird auf a, 1 auf -a [Amplitude] gemappt)
    std::array<float, COLS*SCALE> llr = {};
    MapToSignalVector(codeword, llr, 
                      arguments.getAmplitude());

    // 2: Add noise (Binary Symmetric / Gaussian Noise / Binary Erasure )
    // je nachdem ob als Konsolenargument übergeben (Default: nur Gaussian Noise)
    if (arguments.isBSCEnabled()){
            BinarySymmetric bsc(arguments.getBSCProb());
            bsc.applyNoise(llr);
    }
    if (arguments.isAWGNEnabled()){
            GaussianNoise gn(arguments.getSNR(), 
                            arguments.getAmplitude());     //(SNR in dB, a Amplitude) 
            gn.applyNoise(llr); 
    }
    if (arguments.isErasureEnabled()){
            BinaryErasure bec (arguments.getBECProb());
            bec.applyNoise(llr); 
    }
    if (arguments.isBurstEnabled()){
            BurstError burst(arguments.getErrorRate(),
                            arguments.getBadProb(),
                            arguments.getGoodProb());
            burst.applyNoise(llr);
            burst.statistics();
    }
 //Vorbereitung auf Decoder (Kantenliste wird berechnet)
    // LLR das während der Iteration verändert wird: extrinsic term
    std::array<float, COLS*SCALE> current_llr = llr;
    
    std::vector<CheckNode> check_nodes(ROWS*SCALE);
    FillCNConnections(base, check_nodes);

//Deoder
    size_t iterate = 50;
    std::bitset<COLS*SCALE> calc_codeword;
    std::array<int,ROWS*SCALE> syndrom = {};
    bool parity_failed;

    for (size_t i = 0; i < iterate; i++) {

        syndrom.fill(0); //neuer Versuch, neues Glück..

        parity_failed = false;
        
        // ---- Check Node Update
        MinAndSign(current_llr, check_nodes);
        CheckNodeUpdate(current_llr, check_nodes);

        // ---- Variable Node Update
        // LLR's std::array<float, COLS*SCALE> llr wird aktualisiert
        VarNodeUpdate(llr, current_llr, check_nodes);

        // CHECK OB DIE LÖSUNG SCHON GEFUNDEN
        HardDecision (current_llr, calc_codeword);

        
        for (size_t cn = 0; cn < check_nodes.size(); cn++) {
           
           for (size_t n = 0; n < check_nodes[cn].neighbors.size(); n++){
            
                syndrom[cn] =  syndrom[cn] xor calc_codeword[check_nodes[cn].neighbors[n]];

                }

            if (syndrom[cn] != 0) {
                    //std::cout << "Iteration # " << i << std::endl;
                    //std::cout << "Failed! " << std::endl;
                    parity_failed = true;
                    break;
            }               
        }

        if (parity_failed == false) {
            std::cout << "YAAY! Die Nachricht wurde in der " << i << ". Iteration dekodiert" << std::endl;

                for (size_t i = 0; i < calc_codeword.size(); i++){
                    std::cout << +calc_codeword[i];
                }               
                    break;
            } 

    }

// MESSAGEDISPATCHER HAMMING DISTANZ BERECHNEN!!
// wenns userinput, gebe die nachricht wieder als ascii (calc_codeword ist ein bitset!!)
std::cout << "\n\n Rekonstruierte Nachricht: " << std::endl;

for (size_t i = 0; i < 4; i++) { // da message 256 Bit ist und aus 64bit Elementen besteht 256/64=4 
    std::bitset<64> chunk;

    for (size_t bitIndex = 0; bitIndex < 64; bitIndex++) {
        if (calc_codeword[i * 64 + bitIndex]) {
            chunk.set(bitIndex); // bit an Position bitIndex auf 1 setzen
        }
    }

    // 64 Bits in eine Zahl umwandeln
    uint64_t rawData = chunk.to_ullong(); 

    // ascii Zeichen ausgeben
    uint8_t* bytePtr = reinterpret_cast<uint8_t*>(&rawData);
    for (size_t b = 0; b < 8; b++) {
        if (bytePtr[b] != 0) {
            std::cout << static_cast<char>(bytePtr[b]);
        }
    }
}
std::cout << std::endl;

    }


    return 0;
}
