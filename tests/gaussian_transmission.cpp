// Small Test: Transmission Through a Gaussian Channel

#include <iostream>

// GAUSSIAN_distribution
#include <iostream>
#include <random>


float ComputeStdDev(float a, uint8_t snr ){
    float code_rate = 0.5f; // sollte sonst WOANDERS definiert werden!!!!!!!!!!!!!!!!!!!!
    
    float energy_per_coded_bit = a*a/ code_rate; //formel = a hoch 2 / code_rate
    float n_0 = energy_per_coded_bit / snr;
    
    float stddev = std::sqrt(n_0 / 2);
    return stddev;
}

void ChannelPosterior (){
    std::cout << "gaba " << std::endl;
}

int main()
{


    // Sind Notwendig um Rest abzuleiten
    // Gesucht werden: Standardabweichung fürs Modellieren des Rauschens
    float a = 1.0f; // Amplitude
    float snr = 1.0f; // SNR Linear
    float stddev= ComputeStdDev (a,snr);

    
    // array Codeword z.B
    std::array<int, 10> codeword = {0,1,0,0,1,1,0,1,1,0};
    
    // ---------------------------------------  Map to a Signal VEctor
    std::array<float, 10> t = {};
    
    for (int i = 0; i<codeword.size(); i++){
        codeword[i] == 0? t[i]=-a : t[i]=a;
        //std::cout << t[i] << std::endl;
    }
    
    // ----------------------------------  Recieved signal (aka add noise to transmitted vector)
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(0.0,stddev);
    
    std::array<float, 10> r = {}; // recieved vector
    
    // test //std::array<float, 10> r = {-0.63, -0.83, -0.73, -0.04, 0.1, 0.95, -0.76, 0.66, -0.55, 0.58};
    
    for (int i = 0; i<t.size(); i++){
        
        r[i] = t[i] + distribution(generator);
        std::cout << t[i] << " " << r[i] << std::endl;
    }
    
    // --------------------------- Compute Channel Reliabiliry
    // Formel: 2 * Wurzel aus Ec/ sigma hoch 2
    // Ec = a hoch 2
    //     float ch_rel = (2.0f  * std::sqrt(a*a))/ (stddev*stddev); aber sqrt(a*a) = a
    float ch_rel = (2.0f  * a)/ (stddev*stddev);
    
    //LLR's
    std::array<float, 10> llr = {};
    for (int i=0; i < llr.size(); i++){
        llr[i] = ch_rel * r[i];
    }
    
    // ------------------------------- Compute Channel Posterior Probability
    //std::array<float, 10> ch_post = {};
    //for (int i = 0; i<ch_post.size(); i++){
    //    ch_post[i] = (1 / (1 + std::exp((-2*a*r[i])/(stddev*stddev))));
    //}

  return 0;
}



