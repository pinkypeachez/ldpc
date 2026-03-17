#pragma once

#include <string>
#include <vector>
#include <array>
#include <cstdint>

class MessageDispatcher {

    private:
        std::string inputBuffer_; 


        void FillChunksWithInput();

    public:

         void dispatch(const std::string& userInput);
         std::vector<std::array<uint64_t, 4>> chunks_; // 4 * 64bit = 256bit pro Chunk
         size_t n_batch_ = 0;

};