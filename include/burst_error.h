#pragma once

#include "noisy_channel.h"

class BurstError : public INoisyChannel {
    private:
     float errorRateBurst_;
     float badProb_;
     float goodProb_;
     bool state_; //um sich an bursts über mehrere Chunks erinnern zu können, 1 GOOD, 0 BAD

    public:
     int numberBursts = 0;
     std::vector<size_t> burstLength;
     BurstError(float errorRateBurst_, float badProb_, float goodProb_);

     void applyNoise( std::array<float, params::COLS*params::SCALE>& llr)
             override;
    
     void statistics() override;




};