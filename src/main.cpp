#include <iostream>
#include <random>
#include <cmath> // fürs aufrunden
#include <string> // um teile vom string zu nehmen

#include "preprocessing.h"
#include "encoder.h"
#include "noisy_channel.h"
#include "gaussianNoise.h"
#include "binary_erasure.h"
#include "binary_symmetric.h"
#include "burst_error.h"
#include "decoder.h"
#include "params.h"
#include "input.h"

using namespace params;


int main() {
    // ======================================= PREPROCESSING STAGE ======================================= 
    // 1. Base Matrix wird erstellt:
    // Links (Message-Teil): wird mit zufälligen Shift Werden gefüllt - FillMessagePart
    // Rehts (Parity-Teil): Dual Diagonal Form - FillParityPart
    // 2. Girth-4 Check durchgeführt - girthCheck

    std::random_device rd;
    uint seed = rd();
    std::mt19937 generator {seed};
    std::cout << " Seed für mt19937 Generator ist " << seed << std::endl;

    std::cout << " Base Matrix wird gefüllt " << std::endl;

    int8_t base[ROWS][COLS] = {};
    FillParityPart(base); //Dual Diagonal Form
    FillMessagePart(base,generator); // CN Degree ist gleich, ausgenommen 0.Zeile (bzw.0-SCALE Zeilen) - Quasi Irregular LDPC


    std::cout << "\n ======= Girth-4 Check wird durchgeführt..." << std::endl;
    bool girthOk = false;

    while (!girthOk) {
        std::cout << "Girth-4 Check NICHT bestanden, neuer Versuch..." << std::endl;
        FillMessagePart(base, generator);
        girthOk = girthCheck(base);
    }

    std::cout << "\n\nGirth-4 Check bestanden!" << std::endl;
    std::cout << "\n ======= Base Matrix ist: " << std::endl;
    for (size_t i = 0; i < ROWS; i++){
        std::cout << " " << std::endl;
        for (size_t j = 0; j < COLS; j++) {
            std::cout << +base[i][j] << " ";     
        }
    } 


 



    // =============== TEST (statt mit dem hardcoded (inhaltlich sinnlosen) bit stream zu arbeiten, als Eingabe char-String nehmen)  ======================================= 

    //std::string input = "gaba baka kapapagergerg regergergerg regergergerg p";
    std::string input;
    std::cout << "\n Geben Sie die Nachricht ein, die übertragen werden soll: " << std::endl;
    std::getline(std::cin, input);

    std::cout << "Die Nachricht ist " << input.size() << " Bytes groß" << std::endl;

    MessageDispatcher m;
    m.dispatch(input);

    std::array<uint64_t,COLS-ROWS> message = m.chunks_[0];

/*     // Ausgabe der zu übertragenden Nachricht (gepadded, in binary)
for (size_t i = 0; i < m.n_batch_; i++){
        for (size_t j = 0; j < m.chunks_[i].size(); j++){

         std::cout << std::bitset<64>(m.chunks_[i][j]) << std::endl;
        }
    } */


   // ======================================= ENCODER STAGE  ======================================= 
/* AUSKOMMENTIEREN!!!    std::array<uint64_t,COLS-ROWS> message= {
        0b0110110001010100011010100111001001101101011101101110110010110010,
        0b0110110001010001011010100111010101101101011100000110110011010010,
        0b0100111001010010011010100111000011101111011101100110110011110010,
        0b0010110001010000111010100111001011101101011100100110110001110011
    }; */
    std::array<uint64_t,ROWS> parity= {};

    ComputeParity(base, message, parity);


/*     for (int8_t i = 0; i < ROWS; i++){
        cout << std::bitset<64>(parity[i]) << endl;
    } */



// ----- Message Bits & Parity Bits werden zusammengefügt
    std::array<uint64_t, COLS> codeword;

    std::copy(message.begin(), message.end(), codeword.begin());
    std::copy(parity.begin(), parity.end(), codeword.begin() + (COLS-ROWS)); 


   bool encoderOk = CheckCodeword(base, codeword);

    if (encoderOk)
       std::cout << "Encoder ist mathematisch korrekt (H*c = 0)" << std::endl;
    else
       std::cout << "FEHLER IM ENCODER" << std::endl;

    
    
    // AUSGABE VON CODEWORD (message + parity)
    std::cout << "Codeword " << std::endl;
    for (size_t i = 0; i<COLS; i++){
      std::cout << "i " << i << ": " << std::bitset<64>(codeword[i]) << std::endl;
    }    



    // ======================================= NOISY CHANNEL 
    float a = 2.0f; //Amplitude

     // 1: Map to a Signal Vector (0 wird auf a, 1 auf -a [Amplitude] gemappt)
    std::array<float, COLS*SCALE> llr = {};
    MapToSignalVector(codeword, llr, a);

    // 2: Add noise (Binary Symmetric / Gaussian Noise / Binary Erasure )
    BinarySymmetric bsc(0.2f);
    bsc.applyNoise(llr);

    //GaussianNoise(SNR in dB, Amplitude) 
    GaussianNoise gn(5.0f, a);
    gn.applyNoise(llr); 

    BinaryErasure bec(0.2f);
    bec.applyNoise(llr); 

    BurstError burst(0.5f,0.01f,0.1f);
    burst.applyNoise(llr);
    burst.statistics();

 // --------------------------------------- VORBEREITUNG AUF DECODER: KANTENLISTE BERECHNEN
    
    // LLR das während der Iteration verändert wird: extrinsic term
    std::array<float, COLS*SCALE> current_llr = llr;
    
    std::vector<CheckNode> check_nodes(ROWS*SCALE);
    FillCNConnections(base, check_nodes);

/*  // AUSGABE PARITY CHECK BIT : dazugehörige VNs 
    for (size_t node = 0; node < check_nodes.size(); node++){
        std::cout << "\n Node " << node << " : " << std::endl;
        for (size_t vn = 0; vn < check_nodes[node].neighbors.size(); vn++){
            std::cout << check_nodes[node].neighbors[vn] << " ";
        }
    }  */


    




    // ======================================= DECODER STAGE ==============================================
    
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
        // Option1: Hard Decision
        HardDecision (current_llr, calc_codeword);

        
        for (size_t cn = 0; cn < check_nodes.size(); cn++) {
            //std::cout << "Codeword " << std::endl;
           for (size_t n = 0; n < check_nodes[cn].neighbors.size(); n++){
            
                syndrom[cn] =  syndrom[cn] xor calc_codeword[check_nodes[cn].neighbors[n]];

                //std::cout << syndrom[cn];
                //std::cout << calc_codeword[check_nodes[cn].neighbors[n]] << std::endl;
                }
            //std::cout << " " << std::endl;    
            //std::cout << syndrom[cn] << std::endl;
            if (syndrom[cn] != 0) {
                    //std::cout << "Iteration # " << i << std::endl;
                    std::cout << "Failed! " << std::endl;
                    parity_failed = true;
                    break;
            }
      
                    
                
        }

        if (parity_failed == false) {
            std::cout << "YAAAY! Bei Iteration " << i << std::endl;

                for (size_t i = 0; i < calc_codeword.size(); i++){
                    std::cout << +calc_codeword[i];
                }               
                    break;
            } 

    }

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




    return 0;
}
