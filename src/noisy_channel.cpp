#include "noisy_channel.h"

// bernoulli_distribution (BSC) oder normal_distribution (Gaussian)
#include <iostream>
#include <random>

using namespace std;
using namespace params;

// die Daten werden in 512-bit Batches verarbeitet

void binary_symmetric(std::array<uint64_t,COLS> &codeword, float noise_level, std::vector <int>& flipped_bits){
    cout << "Binary Symmetric Noise Channel" << endl;
    
    // Static damit Generator nicht bei jedem Funktionsaufruf neu startet
  static std::default_random_engine generator;
  std::bernoulli_distribution distribution(noise_level); // Noise Level p=0.1

  bitset<64> noise;
  for (size_t i = 0; i < COLS; i++){ 
    for (size_t j = 0; j < 64; j++){ //64-Bit Noise Sequenzen werden erzeugt 
        noise[j] = distribution(generator);
        // KORREKTUR LÖSCHEN! debugging
        if (noise[j] == 1){
          //std::cout << "Bit " << i*64 + j << " wird geflippt " << std::endl;
          flipped_bits.push_back(i*64 + j);
        }
    } 
    //cout << "Noise: " << bitset<64>(noise) << endl;
    //cout << "Davor: " << bitset<64>(codeword[i]) << endl;

    codeword[i] =  (noise xor bitset<64>(codeword[i])).to_ullong();
    //cout << "Danach: " << bitset<64>(codeword[i]) << endl;
    // oder alternativ codeword[i] ^= noise.to_ullong(); 

  } 
}
    
// ========================= GAUSSIAN ========================= 
float ComputeStdDev(float a, float snr_linear ){
  // Gesucht wird: Standardabweichung fürs Modellieren des Rauschens
    //float code_rate = 0.5f; // KORREKTUR sollte sonst WOANDERS definiert werden!!!!!!!!!!!!!!!!!!!!
    float code_rate = static_cast<float>(COLS-ROWS)/static_cast<float>(COLS);

    // Energy pro gesendeten Symbol (coded bit)
    // Formel umstellen: ((-a)*(-a) + a*a) / 2 ----> 2*(a*a)/ 2 ----> a*a
    float Es = a*a;

    // Energy pro Message bit 
    float Eb = Es/ code_rate; 

    float n_0 = Eb / snr_linear;
    
    float stddev = std::sqrt(n_0 / 2.0f);
    return stddev;
}


  
void GaussianNoise(const std::array<uint64_t, COLS> &codeword, 
              std::array<float, COLS*SCALE>& r,
              const float stddev,
              const float a){

    cout << "Gaussian Noise Channel" << endl;

     // -----------------  "Map to a Signal VEctor"
     // 0 wird auf a, 1 auf -a [Amplitude] gemappt
    std::array<float, COLS*SCALE> t = {}; // 512 Bit groß, flacher Vektor

    for (size_t i = 0; i<COLS; i++){
      bitset<64> current_codeword = bitset<64>(codeword[i]);

      for (size_t j = 0; j < 64; j++){ 
        bool bit = current_codeword[63 - j]; // Wegen Endianess
        bit == 0 ? t[64*i+j]=a : t[64*i+j]=-a;
        //std::cout << count <<  " - codeword[i]" << (current_codeword)[j] << " " << "t[i]:" << t[SCALE*i+j] << std::endl;
      }
    }

    // -----------------  Recieved signal (aka add gaussian noise to transmitted vector)
    // KORREKTUR generator welches Scope am besten?
    static std::default_random_engine generator;
    std::normal_distribution<float> distribution(0.0,stddev);
    
    for (size_t i = 0; i<t.size(); i++){
        
        r[i] = t[i] + distribution(generator);
        //std::cout << t[i] << " " << r[i] << std::endl;
    } 

    }
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 











