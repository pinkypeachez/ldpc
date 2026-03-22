#include "binary_symmetric.h"

#include <random>

BinarySymmetric::BinarySymmetric(float flipProb) : flipProb_(flipProb) {}

void BinarySymmetric::statistics() {
     
    }

    
void BinarySymmetric::applyNoise(std::array<float, params::COLS*params::SCALE>& llr){
  static std::default_random_engine generator;
  std::bernoulli_distribution distribution(flipProb_); // Noise Level p=0.1
  

  for (size_t i = 0; i < llr.size(); i++){
    if (distribution(generator) == 1) {
        llr[i] *= (-1);
    }
  }


}