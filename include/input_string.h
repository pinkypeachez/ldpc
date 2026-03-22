#pragma once

#include <string>
#include <vector>
#include <array>
#include <cstdint>


/* MessageDispatcher Class
- teilt den Input-String in Chunks (1 Chunk 256 Message bits)
- beim Bedarf wird die Nachricht mit 0 gepaddet (um auf die chunk Größe zu kommen) */
class MessageDispatcher {

    private:
        std::string inputBuffer_; 


        void FillChunksWithInput();

    public:

         void dispatch(const std::string& userInput);
         std::vector<std::array<uint64_t, 4>> chunks; // 4 * 64bit = 256bit pro Chunk
         size_t numberOfChunks = 0;
        // void hammingDistance(std::array<uint64_t, 4>& chunk);

};