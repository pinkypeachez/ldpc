#include "noisy_channel.h"

// bernoulli_distribution (BSC) oder normal_distribution (Gaussian)
#include <iostream>
#include <random>

using namespace std;
using namespace params;


void MapToSignalVector(const std::array<uint64_t, params::COLS> &codeword, 
              std::array<float, params::COLS*params::SCALE>& llr, const float a){
       // 0 wird auf a, 1 auf -a [Amplitude] gemappt
    for (size_t i = 0; i<params::COLS; i++){ // es wäre i<codeword.size() besser KORREKTUR
      std::bitset<64> current_codeword = std::bitset<64>(codeword[i]);

      for (size_t j = 0; j < 64; j++){ 
        bool bit = current_codeword[j];
        bit == 0 ? llr[64*i+j]=a : llr[64*i+j]=-a;
      }
    }
}


    
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 











