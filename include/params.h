// Parameter für LDPC 
#pragma once
namespace params {
    inline constexpr int SCALE = 64; //mit einem anderen Scale wird es NICHT funktionieren
    inline constexpr int COLS  = 8;
    inline constexpr int ROWS  = 4;
    inline constexpr float CODE_RATE = static_cast<float>(COLS-ROWS)/static_cast<float>(COLS);
}

