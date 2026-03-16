#include <iostream>
#include <random>
#include <cmath> // fürs aufrunden
#include <string> // um teile vom string zu nehmen

#include "preprocessing.h"
#include "encoder.h"
#include "noisy_channel.h"
#include "decoder.h"
#include "params.h"

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
    FillParityPart(base);
    FillMessagePart(base,generator);

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
    std::cout << input << std::endl;


    //InputToMessageStruct();

    std::cout << "Adresse " << &input << std::endl;
    //std::cout << "Geben Sie die zu übertragende Nachricht ein: " << std::endl;
    //std::cin >> input;
    size_t capacity = input.capacity();
    size_t size = input.size();
    std::cout << "Message size: " << input.capacity() << std::endl;
    std::cout << "Message size: " << input.size() << std::endl;

    //Wie viele "message" Blöcke sollen verarbeitet werden?
    float n_batch = std::ceil(input.size()/(32.0f));
    std::cout << n_batch << " Batches " << std::endl;

    size_t rest = input.size() % 32; // wie viele Elemente sind im letzten Batch?

    std::array<uint64_t,COLS-ROWS> new_m= {};

// Fülle die Message Block mit den Werten

/*     for (size_t batch = 0; batch < n_batch; batch++){
     for (size_t i = 0; i < new_m.size(); i++){
            if (i != n_batch) {
                std::string bit64 = input.substr(8*i, 8*i+8);
                new_m[i] = std::bitset<64>(bit64).to_ullong();
                std::cout << std::bitset<64>(new_m[i]) <<std::endl;

        } else {
            std::cout << "sheeeet " << std::endl;
        }

    }
    } */

   // ======================================= ENCODER STAGE  ======================================= 
    std::array<uint64_t,COLS-ROWS> message= {
        0b0110110001010100011010100111001001101101011101101110110010110010,
        0b0110110001010001011010100111010101101101011100000110110011010010,
        0b0100111001010010011010100111000011101111011101100110110011110010,
        0b0010110001010000111010100111001011101101011100100110110001110011
    };
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

/*      // ---------- WENN BINARY SYMMETRIC
   float noise_level = 0.01f;

    //FÜR DEBUGGING!!! Um Indizes von geflippten Bits zu kennen
    std::vector <int> flipped_bits;

    binary_symmetric(codeword, noise_level, flipped_bits);
   

    // auf r-Vektor mappen -1 für  1, +1 für 0
    std::bitset<64> bits;
    for (size_t i = 0; i < COLS; i++) {
        bits = codeword[i];
        for (size_t j = 0; j < 64; j++) {
            bits[j] == 1 ? r[i * 64 + j] = -1.0f: r[i * 64 + j] = 1.0f;
            std::cout << r[i * 64 + j] << std::endl;
        }
    }

    float ch_rel = std::log((1.0f-noise_level)/noise_level);
     */
    
    // KORREKTUR bitte auch in params datei!!!!!!
 // SNR wird meistens in dB angegeben
    const float a = 1.0f; // Amplitude
    const float snr_db = 5.0f; // SNR in dB
    const float snr_linear = std::pow(10.0f, snr_db / 10.0f);
    const float stddev = ComputeStdDev (a,snr_linear);
    //  float stddev = 0.000000001f; test
    std::cout << "STDDEV: " << stddev << std::endl;
    
    std::array<float, COLS*SCALE> r = {}; // recieved vector (signal + gaussian noise)
    std::cout << "\n =========== Gaussian Noise Channel =========== " << std::endl;
    GaussianNoise(codeword, r, stddev, a); 

/*     // AUSGABE VON DEM VERRAUSCHTEN CODEWORD  (codeword + gaussian noise)
    std::cout << "Recieved Codeword (r): " << std::endl;
    for (size_t i = 0; i<r.size(); i++){
      std::cout << r[i] << " ";
      if (i % 63 == 0 and i != 0){
        std::cout << " " << std::endl;
      }
    }   */




    // --------------------------------------- VORBEREITUNG AUF DECODER: LLR BERECHNEN, KANTENLISTE BERECHNEN

    // Compute Channel Reliability
    // Formel: 2 * Wurzel aus Ec/ sigma hoch 2
    // Ec = a hoch 2
    //     float ch_rel = (2.0f  * std::sqrt(a*a))/ (stddev*stddev); aber sqrt(a*a) = a
    
    
    float ch_rel = (2.0f  * a)/ (stddev*stddev); 

    
    //LLR's: initial
    // Channel LLR: intrinsic term
    std::array<float, COLS*SCALE> llr = {};
    for (size_t i=0; i < r.size(); i++){
        llr[i] = ch_rel * r[i];
        //std::cout << llr[i] << std::endl;
    } 

    // LLR das während der Iteration verändert wird: extrinsic term
    std::array<float, COLS*SCALE> current_llr = llr;

    std::vector<CheckNode> check_nodes(ROWS*SCALE);
    FillCNConnections(base, check_nodes);

/* // AUSGABE PARITY CHECK BIT --> WELCHE VNs SCHÜTZT ER?
    for (size_t node = 0; node < check_nodes.size(); node++){
        std::cout << "\n Node " << node << " : " << std::endl;
        for (size_t vn = 0; vn < check_nodes[node].neighbors.size(); vn++){
            std::cout << check_nodes[node].neighbors[vn] << " ";
        }
    } */


    




    // ======================================= DECODER STAGE ==============================================
    
    size_t iterate = 50;
    std::array<int, COLS*SCALE> calc_codeword = {};
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

    return 0;
}
