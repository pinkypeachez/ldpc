#include "burst_error.h"
#include <random>
#include <iostream>

//Konstruktor
BurstError::BurstError(float errorRateBurst, float badProb, float goodProb)
  : errorRateBurst_(errorRateBurst), badProb_(badProb), goodProb_(goodProb) {}

void BurstError::statistics() {
    // std::cout << "[PARAMETER]\n";
    // std::cout << "    Erasure Probability in a BAD State: " << errorRateBurst_ << std::endl;
    // std::cout << "    Probability GOOD --> BAD: " << badProb_ << std::endl;
    // std::cout << "    Probability BAD --> GOOD: " << goodProb_ << std::endl;
     std::cout << "[STATISTICS]\n";
     std::cout << "    Number of Bursts: " << numberBursts << std::endl;
     std::cout << "    With Length: ";
     for (size_t i = 0; i < burstLength.size();i++){
        std::cout << burstLength[i] << " ";
     }
     std::cout << "\n";
    }

    
//Gilbert Elliott Noise Modell
void BurstError::applyNoise( std::array<float, params::COLS*params::SCALE>& llr){
  static std::default_random_engine generator;
  std::bernoulli_distribution errorDist(errorRateBurst_);
  std::bernoulli_distribution badDist(badProb_);
  std::bernoulli_distribution goodDist(goodProb_);
  size_t lengthCount = 0;

  for (size_t i = 0; i < llr.size(); i++){
    //std::cout << " ===== " << std::endl; //debugging das steht für nächstes i
     
     if (state_ == 1){ //Wenn im Good State:
        llr[i] = llr[i];// ist bestimmt DUMM. man kanns weglassen, LLR bleibt so wie er 
        if (badDist(generator) == 1){
            state_ = 0;
        }
    }else { // Wenn im Bad State:
        llr[i] *= static_cast<float>(1-errorDist(generator)); 
        lengthCount++;
        
        if (goodDist(generator) == 1){
            state_ = 1;
            numberBursts ++; //zähle wie viele Bursts aufgetreten sind (für Statistik)
            burstLength.push_back(lengthCount); //füge die Länge des jeweiligen Bursts
            lengthCount = 0; 

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
    

