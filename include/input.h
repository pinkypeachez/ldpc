#pragma once

#include <string>
#include <vector>
#include <array>
#include <cstdint>

// ArgumentParser Klasse
class ArgumentParser {
private:
    std::string input_ = "It's BMW, not 'B M Double You', Bayerische Motorenwerke. Wait what? BAYERISCHE MOTORENWERKE!!!!!!";
    void showHelpAndExit();
    int argc_;
    char** argv_;
    float snr_ = 5.0f;
    int a_ = 1;
    float p_bsc_ = 0.1f;
    float p_bec_ = 0.1f;

    //Gilbert-Elliot-Burst
    float errorRateBurst_ = 0.5f;
    float badProb_ = 0.01f;
    float goodProb_ = 0.1f;

public:
    ArgumentParser(int argc, char* argv[]);
    
    void parse();

    // Getter für main
  //  const std::string& getInput() const { return userInput_; }
   // const std::vector<ChannelConfig>& getChannels() const { return selectedChannels_; }
};

// MessageDispatcher Klasse: User-Input wird vorbereitet
class MessageDispatcher {

    private:
        std::string inputBuffer_; 


        void FillChunksWithInput();

    public:

         void dispatch(const std::string& userInput);
         std::vector<std::array<uint64_t, 4>> chunks_; // 4 * 64bit = 256bit pro Chunk
         size_t n_batch_ = 0;

};