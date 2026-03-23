#pragma once

#include "params.h"

#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <bitset>


/* MessageDispatcher Class
- teilt den Input-String in Chunks (1 Chunk 256 Message bits)
- beim Bedarf wird die Nachricht mit 0 gepaddet (um auf die chunk Größe zu kommen) */
class MessageDispatcher {

    private:
        std::string inputBuffer_; 
        void FillChunksWithInput();
        std::string decoded_message_;;

    public:

         void dispatch(const std::string& userInput);
         std::vector<std::array<uint64_t, 4>> chunks; // 4 * 64bit = 256bit pro Chunk
         size_t numberOfChunks = 0;

//Methoden für Decoded Input String
         void hammingDistance(const std::array<uint64_t, params::COLS>& codeword, const std::bitset<params::COLS*params::SCALE>& calc_codeword);
         void toAscii(std::bitset<params::COLS*params::SCALE> calc_codeword, bool noAppend);
      

         std::string getDecoded() const { return decoded_message_; };

};