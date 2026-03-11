#include <iostream>
#include <random>

#include "preprocessing.h"
#include "encoder.h"
#include "noisy_channel.h"
#include "decoder.h"
#include "params.h"

using namespace params;


int main() {
    // ------------- PREPROCESSING STAGE
    // Base Matrix wird erstellt, Girth-4 Check durchgeführt

    //  --- Initialisierung des Zufallszahlgenerators
    std::random_device rd;
    uint seed = rd();
    std::mt19937 generator {seed};
    std::cout << " Seed für mt19937 Generator ist " << seed << std::endl;

    std::cout << " Base Matrix wird gefüllt " << std::endl;

    int8_t base[ROWS][COLS] = {};

    bool girthOk = false;

    while (!girthOk) {
        std::cout << "Girth-4 Check NICHT bestanden, neuer Versuch..." << std::endl;
        fillMatrix(base, generator);
        girthOk = girthCheck(base);
    }
    
    std::cout << "Girth-4 Check bestanden!" << std::endl;


    // ======================================= ENCODER STAGE  ======================================= 
    std::array<uint64_t,ROWS> message= {
        0b0110110001010100011010100111001001101101011100000110110010110010,
        0b0110010001010001011010100111010001101101011100000110110011010010,
        0b0100111001010000011010100111000011101111011101100110110011110010,
        0b0110110001010000011010100111001001101101011100000110110001110010
    };
    std::array<uint64_t,ROWS> parity= {};


    compute_parity(base, message, parity);

/*     for (int8_t i = 0; i < ROWS; i++){
        cout << std::bitset<64>(parity[i]) << endl;
    } */



// ----- Message Bits & Parity Bits werden zusammengefügt
    std::array<uint64_t, COLS> codeword;
    std::copy(message.begin(), message.end(), codeword.begin());
    std::copy(parity.begin(), parity.end(), codeword.begin() + (COLS-ROWS)); 


    for (size_t i = 0; i < COLS; i++){
        std::cout << std::bitset<64>(codeword[i]) << std::endl;
    }

    std::cout << "Codeword size: " << codeword.size() << std::endl;

     // ------------------------------------------ NOISY CHANNEL 
    // ADD NOISE
    //binary_symmetric(codeword);
    std::array<float, COLS*SCALE> r = {}; // recieved vector (signal + gaussian noise)
    
    // KORREKTUR bitte auch in params datei!!!!!!
    float a = 1.0f; // Amplitude
    float snr = 1.0f; // SNR Linear
    float stddev = ComputeStdDev (a,snr);
    std::cout << "STDDEV: " << stddev << std::endl;
    GaussianNoise(codeword, r, stddev, a);

    // --------------------------------------- VORBEREITUNG AUF DECODER: LLR BERECHNEN, KANTENLISTE BERECHNEN

    // Compute Channel Reliabiliry
    // Formel: 2 * Wurzel aus Ec/ sigma hoch 2
    // Ec = a hoch 2
    //     float ch_rel = (2.0f  * std::sqrt(a*a))/ (stddev*stddev); aber sqrt(a*a) = a
    
    float ch_rel = (2.0f  * a)/ (stddev*stddev);
    
    //LLR's
    std::array<float, COLS*SCALE> llr = {};
    for (size_t i=0; i < llr.size(); i++){
        llr[i] = ch_rel * r[i];
        //std::cout << llr[i] << std::endl;
    } 


    // ======================================= DECODER STAGE ==============================================
/*  OLD:   std::vector <size_t> nodelist;
    nodelist.reserve(ROWS*COLS); // durch Dual Diagonal Form wird eig mehr als nötig allociert

    FillCNConnections(base,nodelist); */

    std::vector<CheckNode> check_nodes(ROWS*SCALE);
    FillCNConnections(base,check_nodes);
    
    // hier wird die Schleife beginnen (ANZAHL ITERATIONEN WÄHLEN)
    // ---- Check Node Update
    MinAndSign(llr, check_nodes);

    // Die Nachricht die von CNs an VNs geschikt wird soll in einem
    // #CN*LLRsize(bzw message size)-großen Array gespeichert werden
    std::vector <float> cn2vn;
    cn2vn.reserve(ROWS*SCALE*COLS); 
    CheckNodeUpdate(llr, check_nodes, cn2vn);


/*     // check
    std::cout << "Check-Node 66 hat " << check_nodes[66].neighbors.size() 
              << " Verbindungen" << std::endl;
    for (size_t i = 0; i < check_nodes[66].neighbors.size() ; i++){
        std::cout << check_nodes[66].neighbors[i] << std::endl;
    }    */       

    
    // ACHTUNG!!! Wegen Dual Diagonal Form hat 1. Check Node -1 weniger Verbindungen als der Rest
    // in Decoder schleife berücksigtigen!

/* ==========================  TEST DECODER: CheckNode Update Step
     Dafür das [0].Element, der LLR Werte nehmen, das quasi alle Verbindungen von CheckNode 0 beinhaltet
    zum Testen des Vorzeichen-Checks + MinSuche */

    // BITTE NICHT VERGESSEN size von test_llr von SCALE auf SCALE*COLS zu verändern!!!!
    
/*     std::array<float, SCALE> test_llr; //= llr[0];

     for (int i = 0; i < test_llr.size(); i++) {
        test_llr[i] = llr[i];
        //std::cout << +(test_llr[i]) << std::endl;
    }  */


// egal ich übergebe den gesamten nodelist....
    //CheckNodeUpdate(test_llr, nodelist);



    

    
    
    return 0;
}
