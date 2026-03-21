#include "burst_error.h"
#include <random>
#include <iostream>

//Konstruktor
BurstError::BurstError(float errorRateBurst, float badProb, float goodProb)
  : errorRateBurst_(errorRateBurst), badProb_(badProb), goodProb_(goodProb) {}


//Gilbert Elliott Noise Modell
void BurstError::applyNoise( std::array<float, params::COLS*params::SCALE>& llr){
  static std::default_random_engine generator;
  std::bernoulli_distribution errorDist(errorRateBurst_);
  std::bernoulli_distribution badDist(badProb_);
  std::bernoulli_distribution goodDist(goodProb_);


  for (size_t i = 0; i < llr.size(); i++){
    //std::cout << " ===== " << std::endl; //debugging das steht für nächstes i
     if (state_ == 1){ //Wenn im Good State:
        llr[i] = llr[i];// ist bestimmt DUMM. man kanns weglassen, LLR bleibt so wie er 
        if (badDist(generator) == 1){
            state_ = 0;
        }
    }else { // Wenn im Bad State:
        llr[i] *= (1-errorDist(generator)); //CAST ZU FLOAT!
        //std::cout << "1 " << std::endl; // debugging: länge von Burst nachvollziehen
        if (goodDist(generator) == 1){
            state_ = 1;
        }
    }

/*         if (errorDist(generator) == 1) { 
            llr[i] = 0;
        }  
--> da ich unnötiges Branching vermeiden will, weise ich dem Outcome vom "errorDist" folgende Bedeutung zu:
wenn 0: erasure passiert (0*LLR[i]-->0)
wenn 1: kein erasure, Wert bleibt (1*LLR[i]-->LLR[i])
insofern kann ich ohne if-else den llr wert berechnen indem ich LLR mit Output von errorDist multipliziere
*/

    


    }

  
  
  }
    

/*     for (size_t i = 0; i < llr.size(); i++){
    if (distribution(generator) == 1) {
     for (size_t j = 0; j < burstLength_; j++){
        llr[i] *= (-1);
     }
     
    }
  } */


