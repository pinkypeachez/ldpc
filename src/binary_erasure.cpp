#include "binary_erasure.h"

#include <random>
#include <iostream>

void BinaryErasure::statistics() {
     
    }

    
void BinaryErasure::applyNoise( 
              std::array<float, params::COLS*params::SCALE>& llr){
 
    // Static damit Generator nicht bei jedem Funktionsaufruf neu startet
  static std::default_random_engine generator;
  std::bernoulli_distribution distribution(erasureProb_); 

  for (size_t i = 0; i < llr.size(); i++){
        if (distribution(generator) == 1){
            llr[i] = 0;
        } 

  } 

}