#include "binary_erasure.h"

#include <random>
#include <iostream>


void BinaryErasure::applyNoise(const std::array<float, params::COLS*params::SCALE>& t,
              std::array<float, params::COLS*params::SCALE>& llr){

    
    // Static damit Generator nicht bei jedem Funktionsaufruf neu startet
  static std::default_random_engine generator;
  std::bernoulli_distribution distribution(erasureProb_); 

  for (size_t i = 0; i < llr.size(); i++){
        if (distribution(generator) == 1){
            llr[i] = 0;
        } 

  }

 /*  for (size_t i =0; i <params::COLS; i++){
        std::bitset<64> current_codeword = std::bitset<64>(codeword[i]);

      for (size_t j = 0; j < 64; j++){ 
        bool bit = current_codeword[j];
        bit == 0 ? llr[64*i+j]=1 : llr[64*i+j]=-1;
        if (distribution(generator) == 1){
            llr[64*i+j] = 0;
        } 

        //std::cout <<  " - codeword[i]" << (current_codeword)[j] << " " << "t[i]:" << t[SCALE*i+j] << std::endl;
      }

    } */
}