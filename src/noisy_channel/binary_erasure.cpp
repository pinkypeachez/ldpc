#include "binary_erasure.h"

#include <random>
#include <iostream>

void BinaryErasure::statistics() {
 std::cout << "[STATISTICS]\n";
     std::cout << "    Number of Erasures: " << numberErasures_ << std::endl;
     std::cout << "\n\n";
    }

void BinaryErasure::getParameter() {
    std::cout << "Binary Erasure Noise:\n";
   // std::cout << "[PARAMETER]\n";
    std::cout << "    Erasure Probability: " << erasureProb_ << std::endl;
    std::cout << "\n";
}
    
void BinaryErasure::applyNoise( 
              std::array<float, params::COLS*params::SCALE>& llr){
 
    // Static damit Generator nicht bei jedem Funktionsaufruf neu startet
  static std::default_random_engine generator;
  std::bernoulli_distribution distribution(erasureProb_); 

  for (size_t i = 0; i < llr.size(); i++){
        if (distribution(generator) == 1){
            llr[i] = 0;
            numberErasures_++;
        } 

  } 

}