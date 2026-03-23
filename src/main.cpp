#include <iostream>
#include <random>
#include <cmath> // fürs aufrunden
#include <string> // um teile vom string zu nehmen
#include <fstream> // für simulation, ergebnisse zu schreiben

#include "argument_parser.h"
#include "preprocessing.h"
#include "encoder.h"
#include "noisy_channel.h"
#include "gaussian_noise.h"
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

// Zufallszahlgenerator   
    std::random_device rd;
    std::mt19937_64 generator( rd() );

// Input-String (oder Default-String) wird in Chunks aufgeteilt
// aber wenn kein Input string - generiere zufällige Message

    MessageDispatcher m;
    if (arguments.randomMessage == false){
        m.dispatch(arguments.getInput());
    } else {
        std::uniform_int_distribution<uint64_t> distribution(
         0, std::numeric_limits<uint64_t>::max());
        m.numberOfChunks = arguments.getNumberChunks();
        m.randomFill(generator,distribution);
    }
/*     std::ofstream logFile("simulation_results.csv");
    logFile << "SNR;HammingDistance" << std::endl; */



// ALLE Noise Channels werden initialisiert, auch die, die später nicht benutzt werden
// (ich habe keine Zeit es schlauer zu lösen sry!!)
    std::cout << "[INFO] NOISE MODELS INITIALISATION" << std::endl;
    BinarySymmetric bsc(arguments.getBSCProb());
    GaussianNoise gn(arguments.getSNR(), arguments.getAmplitude());
    BinaryErasure bec(arguments.getBECProb());
    BurstError burst(arguments.getErrorRate(), arguments.getBadProb(), arguments.getGoodProb());

// Gesetzte Parameter für gewählte Noise Channels ausgegeben
    if (arguments.isBSCEnabled()) bsc.getParameter();
    if (arguments.isAWGNEnabled()) gn.getParameter();
    if (arguments.isErasureEnabled()) bec.getParameter();
    if (arguments.isBurstEnabled()) burst.getParameter();

// Preprocessing: BaseMatrix wird erstellt   
    int8_t base[ROWS][COLS] = {};

    createBaseMatrix(base, generator);

    size_t hammingTotal = 0;

// Die Chunks werden durch Encoder - Noisy Channel - Decoder geschoben
    std::cout << "\n\n[INFO] START DISPATCHING THE CHUNKS\n" << std::endl;
    for (size_t chunk = 0; chunk < m.numberOfChunks; chunk++){
        std::cout << "[INFO] CHUNK #" << chunk << ":\n" << std::endl;
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
    // je nachdem ob als Konsolenargument übergeben (Default: nur Gaussian Noise!!!) DOKUMENTIEREN oder besser gar kein noise..?
    if (arguments.isBSCEnabled()){
            bsc.applyNoise(llr);
    }
    if (arguments.isAWGNEnabled()){
            gn.applyNoise(llr); 
    }
    if (arguments.isErasureEnabled()){
            bec.applyNoise(llr); 
    }
    if (arguments.isBurstEnabled()){
            burst.applyNoise(llr);      
    }

 //Vorbereitung auf Decoder (Kantenliste wird berechnet)
    std::array<float, COLS*SCALE> current_llr = llr;     // LLR das während der Iteration verändert wird: extrinsic term
    
    std::vector<CheckNode> check_nodes(ROWS*SCALE);
    FillCNConnections(base, check_nodes);

//Decoder
    size_t iterate = 100;
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
                    std::cout << "Failed in Iteration " << i;
                    m.hammingDistance(codeword, calc_codeword);
                    parity_failed = true;
                   // LIEBER IN DEBUG FILE
                   //m.toAscii (calc_codeword, parity_failed);
                    //ACHTUNG!!!!! HAMMING DISTANZ WIRD AUF DAS GANZE WORT ANGEWENDET AUSGEGEBEN!
                    //toAscii gibt jedoch message teil aus
                    if (i == iterate-1){
                        std::cout << "The message couldnt be decoded. Please increase the number of iterations or try a different noise model" << std::endl;
                        m.toAscii (calc_codeword, !parity_failed); //auch wenn nicht geklappt, die letzte version hinzufügen
                        hammingTotal += m.hammingDistance(codeword, calc_codeword);
                    }
                    break;
            }               
        }

        if (parity_failed == false) {
            std::cout << "YAAY! Decoded in the " << i << ". Iteration: ";   
            m.toAscii (calc_codeword, parity_failed); 
            hammingTotal += m.hammingDistance(codeword, calc_codeword);
            
            break;
            } 


    }
    }
    std::cout << "\n[INFO] Decoded message: " << std::endl;
    std::cout << m.getDecoded() << std::endl;

    std::cout << "HAMMING TOTAL: " << hammingTotal << std::endl;
   // logFile << arguments.getSNR() << ";" << hammingTotal << std::endl;

    return 0;
}
