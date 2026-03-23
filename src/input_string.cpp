#include "input_string.h"


#include <iostream>
#include <algorithm>
#include <cmath>
#include <bit> // für popcount (hamming Distance)



void MessageDispatcher::FillChunksWithInput()  {

            //Wie viele "message" Blöcke sollen dem LDPC gegeben werden?
            numberOfChunks = std::ceil(inputBuffer_.size()/(32.0f)); //32 weil 256 message bit = 32 Byte sind

            inputBuffer_.resize(32*numberOfChunks, 0); // Padding mit 0
            chunks.resize(numberOfChunks); 

            const uint64_t* inputPointer = reinterpret_cast<const uint64_t*>(inputBuffer_.data());

            for (size_t batch = 0; batch < numberOfChunks; batch++){
        
             for (size_t i = 0; i < 4; i ++){ //4 64bit Elemente passen in "Message" Batch
               chunks[batch][i] = inputPointer[4*batch + i];
             }     
            }


        }

void MessageDispatcher::dispatch(const std::string& userInput) {
    inputBuffer_ = userInput; 
    FillChunksWithInput(); 
    std::cout << "[INFO] Total chunks: " << numberOfChunks << "\n" << std::endl;
}

 void MessageDispatcher::hammingDistance(const std::array<uint64_t, params::COLS>& codeword, const std::bitset<params::COLS*params::SCALE>& calc_codeword) {
/*  //ALTE LÖSUNG
   uint64_t* calcPtr = reinterpret_cast<uint64_t*>(&calc_codeword);

    size_t hammingDistance = 0;
    for (size_t i = 0; i < codeword.size(); i++){
        hammingDistance += std::popcount(codeword[i] ^ calcPtr[i]);
    }
    std::cout << " (Hamming Distance: " << hammingDistance << ")" << std::endl; */

    // sicherer
    std::array<uint64_t, params::COLS> calcArray;
    std::memcpy(calcArray.data(), &calc_codeword, sizeof(calcArray));

    size_t hammingDistance = 0;
    for (size_t i = 0; i < codeword.size(); i++) {
        hammingDistance += std::popcount(codeword[i] ^ calcArray[i]); //xorren und anzahl bit-differenzen zählen --> hamming distance
    }
    std::cout << " (Hamming Distance: " << hammingDistance << ")" << std::endl;

}

void MessageDispatcher::toAscii(std::bitset<params::COLS*params::SCALE> calc_codeword, bool noAppend){

    for (size_t i = 0; i < 4; i++) { // da message 256 Bit ist und aus 64bit Elementen besteht 256/64=4 
        std::bitset<64> chunk;

        for (size_t bitIndex = 0; bitIndex < 64; bitIndex++) {
            if (calc_codeword[i * 64 + bitIndex]) {
                chunk.set(bitIndex); // bit an Position bitIndex auf 1 setzen
            }
        }

        // 64 Bits in eine Zahl umwandeln
        uint64_t rawData = chunk.to_ullong(); 

        // ascii Zeichen ausgeben
        uint8_t* bytePtr = reinterpret_cast<uint8_t*>(&rawData);
        for (size_t b = 0; b < 8; b++) {
            if (bytePtr[b] != 0) {
                std::cout << static_cast<char>(bytePtr[b]); 
                if (noAppend == false) { 
                    decoded_message_.push_back(static_cast<char>(bytePtr[b]));
                   }
            }
        }
    }
    std::cout << std::endl;

}

