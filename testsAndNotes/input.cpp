#include <iostream>
#include <vector>
#include <array>

using namespace std;


int main() {

    std::string input = "gaba baka kapapagergerg regergergerg regergerger ka kapapager";
    std::cout << "Size davor " << input.size() << std::endl;
  
    float n_batch = std::ceil(input.size()/(32.0f)); //32 weil 256 message bit = 32 Byte sind
    input.resize(32*n_batch, 0); //SCHUTZ WENN n_batch = 0 ???????
    std::cout << "Size nach Resize " << input.size() << std::endl;
    std::cout << "Anzahl Batches zum Auffüllen " << n_batch<< std::endl;
    
    // !!!!!!!!!!!!!  n_batch Elemente werden mit 0 gefüllt !!!
    std::vector<std::array<uint64_t, 4>> chunks(n_batch);
    
    

  uint64_t* inputPointer = reinterpret_cast<uint64_t*>(input.data());
  std::cout << "Input Adresse: " << inputPointer << std::endl;
  std::cout << "Inhalt: " << bitset<64>(*(inputPointer)) << std::endl;
// 01100111 g
    for (size_t batch = 0; batch < n_batch; batch++){
        
        std::cout << "\n Batch " << batch << " wird verarbeitet" << std::endl;
        
        for (size_t i = 0; i < 4; i ++){ //4 64bit Elemente passen in "Message" Batch
            std::cout << "===== " << i << ". Element an der adresse " << &inputPointer[4*batch + i] << std::endl;
            
            
            std::cout << bitset<64>(inputPointer[4*batch + i]) << std::endl;
            chunks[batch][i] = inputPointer[4*batch + i];
            
        }
       
        
        
    }
}
