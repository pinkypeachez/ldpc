#include "noisy_channel.h"

// bernoulli_distribution
#include <iostream>
#include <random>

using namespace std;

// die Daten werden in 512-bit Batches verarbeitet

 void binary_symmetric(std::array<uint64_t,COLS> &codeword){
    cout << "Binary Symmetric Noise Channel" << endl;
    
    // Static damit Generator nicht bei jedem Funktionsaufruf neu startet
  static std::default_random_engine generator;
  std::bernoulli_distribution distribution(0.1); // Noise Level

  bitset<64> noise;
  for (int i = 0; i < COLS; i++){ 
    for (int j = 0; j < 64; j++){ //64-Bit Noise Sequenzen werden erzeugt 
        noise[j] = distribution(generator);
    } 
    //cout << "Noise: " << bitset<64>(noise) << endl;
    //cout << "Davor: " << bitset<64>(codeword[i]) << endl;

    codeword[i] =  (noise xor bitset<64>(codeword[i])).to_ullong();
    //cout << "Danach: " << bitset<64>(codeword[i]) << endl;
    // oder alternativ codeword[i] ^= noise.to_ullong(); 
    

    

  } 
}
    
 
 
  
void gaussian(){
    cout << "Gaussian Noise Channel" << endl;
    }
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 











