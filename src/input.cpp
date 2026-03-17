#include "input.h"


#include <iostream>
#include <algorithm>
#include <cmath>

void MessageDispatcher::FillChunksWithInput()  {

            //Wie viele "message" Blöcke sollen dem LDPC gegeben werden?
            n_batch_ = std::ceil(inputBuffer_.size()/(32.0f)); //32 weil 256 message bit = 32 Byte sind

            inputBuffer_.resize(32*n_batch_, 0); // Padding mit 0
            chunks_.resize(n_batch_); 

            uint64_t* inputPointer = reinterpret_cast<uint64_t*>(inputBuffer_.data());

            for (size_t batch = 0; batch < n_batch_; batch++){
        
             for (size_t i = 0; i < 4; i ++){ //4 64bit Elemente passen in "Message" Batch
               chunks_[batch][i] = inputPointer[4*batch + i];
             }     
            }


        }

void MessageDispatcher::dispatch(const std::string& userInput) {
    inputBuffer_ = userInput; 
    FillChunksWithInput(); 
    std::cout << "Nachricht vorbereitet. Batches: " << n_batch_ << std::endl;
}