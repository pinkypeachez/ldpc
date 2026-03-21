#include "gaussianNoise.h"  

#include <bitset>
#include <random>
#include <iostream>

//Konstruktor
GaussianNoise::GaussianNoise(float snr_db, float a) : a_(a), snr_db_(snr_db) 
{
    stddev_ = ComputeStdDev(a_, snr_db_);
    ch_rel_ = (2.0f * a_) / (stddev_ * stddev_);
}

void GaussianNoise::statistics() {
     
    }

    
// Gesucht wird: Standardabweichung fürs Modellieren des Rauschens
float GaussianNoise::ComputeStdDev(const float a, const float snr_db){
   float snr_linear = std::pow(10.0f, snr_db / 10.0f);

    // Energy pro gesendeten Symbol (coded bit)
    // Formel umstellen: ((-a)*(-a) + a*a) / 2 ----> 2*(a*a)/ 2 ----> a*a
    float Es = a*a;

    // Energy pro Message bit 
    float Eb = Es/ params::CODE_RATE; 

    float n_0 = Eb / snr_linear;
    
    return (std::sqrt(n_0 / 2.0f)); //stdtev_
}


  
void GaussianNoise::applyNoise( std::array<float, params::COLS*params::SCALE>& llr){
                
    // -----------------  Recieved signal (aka add gaussian noise to transmitted vector)

    static std::default_random_engine generator;
    std::normal_distribution<float> distribution(0.0,stddev_);
    
    for (size_t i = 0; i<llr.size(); i++){
        llr[i] = (llr[i] + distribution(generator))*ch_rel_;
    } 
}