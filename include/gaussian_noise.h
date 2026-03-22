#pragma once

#include "noisy_channel.h"

#include <array> 
#include <vector> 


class GaussianNoise : public INoisyChannel {
    private:
        float stddev_; 
        float a_;
        float snr_db_;
        float ComputeStdDev(float a_, float snr_db_);
        float ch_rel_;

    /* ch_rel:  Channel Reliability
    Formel: 2 * Wurzel aus Ec/ sigma hoch 2
    Ec = a hoch 2
    float ch_rel = (2.0f  * std::sqrt(a*a))/ (stddev*stddev); aber sqrt(a*a) = a */
    

    public:
        GaussianNoise(float snr_db, float a); //Konstruktor

        void applyNoise( std::array<float, params::COLS*params::SCALE>& llr)
             override;

        void statistics() override;
        void getParameter() override;
};