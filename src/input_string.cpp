#include "input_string.h"

#include <iostream>
#include <algorithm>
#include <cmath>


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

/* void MessageDispatcher::hammingDistance(std::vector<std::array<uint64_t, 4>>& chunks) {

    std::cout << "Hamming Distance" << std::endl;
} */
/* 
void MessageDispatcher::printAscii(){
  for (size_t chunk = 0; chunk < ;chunk++){
  std::cout << "\n\n Rekonstruierte Nachricht: " << std::endl;

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
        }
    }
}
  }
} */