#include "noisy_channel.h"

// bernoulli_distribution (BSC) oder normal_distribution (Gaussian)
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
    
/* void BinaryErasure(const float erasureProb, const std::vector <bool>& noErasureCertainty){
    cout << "Binary Erasure Channel" << endl;
    
    // Static damit Generator nicht bei jedem Funktionsaufruf neu startet
  static std::default_random_engine generator;
  std::bernoulli_distribution distribution(erasureProb); // Erasure Probability p=0.1


  for (size_t i = 0; i < COLS*SCALE; i++){ 
        if (distribution(generator) == 1){
          noErasureCertainty[i] = 0;
          std::cout << "Erasure bei " << i << " ";
        } 
    } 

  }  */





    
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 











