#include "gaussianNoise.h"  

#include <bitset>
#include <random>

//Konstruktor
GaussianNoise::GaussianNoise(float snr_db, float a) : a_(a), snr_db_(snr_db) 
{
    stddev_ = ComputeStdDev(a_, snr_db_);
    float ch_rel_ = (2.0f * a_) / (stddev_ * stddev_);
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


  
void GaussianNoise::applyNoise(const std::array<float, params::COLS*params::SCALE>& t, std::array<float, params::COLS*params::SCALE>& llr){
                
/* void GaussianNoise::applyNoise(const std::array<uint64_t, params::COLS> &codeword, 
              std::array<float, params::COLS*params::SCALE>& r){

 */
/*      // -----------------  "Map to a Signal VEctor"
     // 0 wird auf a, 1 auf -a [Amplitude] gemappt
    std::array<float, params::COLS*params::SCALE> t = {}; // 512 Bit groß, flacher Vektor

    for (size_t i = 0; i<params::COLS; i++){ // es wäre i<codeword.size() besser KORREKTUR
      std::bitset<64> current_codeword = std::bitset<64>(codeword[i]);
      //std::cout << "i " << i << " : "<< bitset<64>(codeword[i]) << std::endl;
    
      for (size_t j = 0; j < 64; j++){ 
        bool bit = current_codeword[j];
        bit == 0 ? t[64*i+j]=a_ : t[64*i+j]=-a_;
        //std::cout <<  " - codeword[i]" << (current_codeword)[j] << " " << "t[i]:" << t[SCALE*i+j] << std::endl;
      }
    } */

    // -----------------  Recieved signal (aka add gaussian noise to transmitted vector)
    static std::default_random_engine generator;
    std::normal_distribution<float> distribution(0.0,stddev_);
    
    for (size_t i = 0; i<t.size(); i++){
        
        llr[i] = (t[i] + distribution(generator))*ch_rel_;
        //std::cout << "t " << t[i] << " --> " << r[i] << std::endl;

/*      ANMERKUNG:   Ich habe eine hardcoded Nachricht "message" definiert und war verwirrt, dass 
        sie in der "umgekehrten Reihenfolge" verarbeitet wird. Sprich Index 0 hat auf das (optisch) letzte (rechte) Zeichen
        des 64-Bit Elements von "message" zugegriffen. ES GIBT ALLERDINGS KEIN PROBLEM!
        Für das Programm liegt LSB am Index 0, und da ich gewohnt bin Texte von links nach rechts 
        zu lesen, dachte ich, Index 0 sei auch links.
        Was ich damit sagen will - es ist eine kleine kognitive Falle.
        Index 0 "fängt" (optisch) rechts an, bzw professioneller gesprochen,  am Index 0 liegt der LSB! */

    } 
              }