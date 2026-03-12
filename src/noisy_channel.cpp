#include "noisy_channel.h"
//#include "encoder.h" // für SCALE const expt KORREKTUR ? LÖSCHEN

// bernoulli_distribution
#include <iostream>
#include <random>

using namespace std;
using namespace params;

// die Daten werden in 512-bit Batches verarbeitet

void binary_symmetric(std::array<uint64_t,COLS> &codeword, float noise_level, std::vector <int>& flipped_bits){
    cout << "Binary Symmetric Noise Channel" << endl;
    
    // Static damit Generator nicht bei jedem Funktionsaufruf neu startet
  static std::default_random_engine generator;
  std::bernoulli_distribution distribution(noise_level); // Noise Level p=0.1

  bitset<64> noise;
  for (size_t i = 0; i < COLS; i++){ 
    for (size_t j = 0; j < 64; j++){ //64-Bit Noise Sequenzen werden erzeugt 
        noise[j] = distribution(generator);
        // KORREKTUR LÖSCHEN! debugging
        if (noise[j] == 1){
          //std::cout << "Bit " << i*64 + j << " wird geflippt " << std::endl;
          flipped_bits.push_back(i*64 + j);
        }
    } 
    //cout << "Noise: " << bitset<64>(noise) << endl;
    //cout << "Davor: " << bitset<64>(codeword[i]) << endl;

    codeword[i] =  (noise xor bitset<64>(codeword[i])).to_ullong();
    //cout << "Danach: " << bitset<64>(codeword[i]) << endl;
    // oder alternativ codeword[i] ^= noise.to_ullong(); 

  } 
}
    
// ======================================================================= GAUSSIAN 
float ComputeStdDev(float a, float snr ){
    float code_rate = 0.5f; // KORREKTUR sollte sonst WOANDERS definiert werden!!!!!!!!!!!!!!!!!!!!
    
    float energy_per_coded_bit = a*a/ code_rate; //formel = a hoch 2 / code_rate
    float n_0 = energy_per_coded_bit / snr;
    
    float stddev = std::sqrt(n_0 / 2);
    return stddev;
}
  
void GaussianNoise(std::array<uint64_t, COLS> &codeword, 
              std::array<float, COLS*SCALE> &r,
              float stddev,
              float a){

    cout << "Gaussian Noise Channel" << endl;

    // KORREKTUR auch Übergabewert? ist das nötig?
     // Gesucht werden: Standardabweichung fürs Modellieren des Rauschens
    

     // ---------------------------------------  Map to a Signal VEctor
    std::array<float, COLS*SCALE> t = {}; // 512 Bit groß, flacher Vektor
    //std::cout << t.size() << "Signal VEctor Size (gemappt) " << std::endl;

    int count = 0;
    for (size_t i = 0; i<COLS; i++){
        //std::cout << bitset<64>(codeword[i]) << std::endl;
      bitset<64> current_codeword = bitset<64>(codeword[i]);
      for (size_t j = 0; j < SCALE; j++){ // 512 - codeword groesse
        current_codeword[j] == 0? t[SCALE*i+j]=-a : t[SCALE*i+j]=a; // KORREKTUR codeword indizes separat vorberechnen
        //std::cout << count <<  " - codeword[i]" << (current_codeword)[j] << " " << "t[i]:" << t[SCALE*i+j] << std::endl;
        count++;
 
    }
    }

    // ----------------------------------  Recieved signal (aka add gaussian noise to transmitted vector)
    // KORREKTUR generator welches Scope am besten?
    static std::default_random_engine generator;
    std::normal_distribution<float> distribution(0.0,stddev);
    
    //std::array<float, COLS*SCALE> r = {}; // recieved vector
    //std::cout << "Recieved Vector Size " << r.size() << std::endl;
    
    for (size_t i = 0; i<t.size(); i++){
        
        r[i] = t[i] + distribution(generator);
        //std::cout << t[i] << " " << r[i] << std::endl;
    } 

    }
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 











