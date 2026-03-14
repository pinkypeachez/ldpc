// Parameter für LDPC 
#ifndef PARAMS_H
#define PARAMS_H

namespace params {
    inline constexpr int SCALE = 64; //mit einem anderen Scale wird es NICHT funktionieren
    inline constexpr int COLS  = 8;
    inline constexpr int ROWS  = 4;
    inline constexpr float CODE_RATE = 0.5f;
}

#endif