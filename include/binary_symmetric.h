#pragma once

#include "noisy_channel.h"


class BinarySymmetric : public INoisyChannel {
    private:
      float flipProb_ = 0.0f;
      size_t numberFlips_ = 0;

    public:
        BinarySymmetric(float flipProb); //Konstruktor

      void applyNoise (std::array<float, params::COLS*params::SCALE>& llr)
             override;
             
      void statistics() override;
      void getParameter() override;
};