#pragma once

#include "noisy_channel.h"
#include "params.h"



class BinaryErasure : public INoisyChannel {
    private: 
      float erasureProb_;

    public:
        BinaryErasure(float erasureProb): erasureProb_(erasureProb) {};

        void applyNoise( std::array<float, params::COLS*params::SCALE>& llr)
             override;

        void statistics() override;
};