#include "binary_symmetric.h"

#include <random>
#include <iostream>

BinarySymmetric::BinarySymmetric(float flipProb) : flipProb_(flipProb) {}

void BinarySymmetric::statistics() {
 
     //std::cout << "[PARAMETER]\n";
    // std::cout << "    Bit Flip Probability: " << flipProb_ << std::endl;
     std::cout << "[STATISTICS]\n";
     std::cout << "    Number of Bit Flips: " << numberFlips_ << std::endl;
     std::cout << "\n\n";
    
    }

void BinarySymmetric::getParameter() {
    std::cout << "Binary Symmetric Channel:\n";
    //std::cout << "[PARAMETER]\n";
    std::cout << "    Bit Flip Probability: " << flipProb_ << std::endl;
    std::cout << "\n";
    }

    
void BinarySymmetric::applyNoise(std::array<float, params::COLS*params::SCALE>& llr){
  static std::default_random_engine generator;
  std::bernoulli_distribution distribution(flipProb_); // Noise Level p=0.1
  

  for (size_t i = 0; i < llr.size(); i++){
    if (distribution(generator) == 1) {
        llr[i] *= (-1);
        numberFlips_++;
    }
  }


}