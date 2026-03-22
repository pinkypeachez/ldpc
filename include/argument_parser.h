#pragma once

#include <string>
#include <cstdint>


/* ArgumentParser Class:
- Konsolenargumente werden geparst, in arguments Klasse gespeichert
- wenn keine Argumente übergeben, startet das Programm mit Default Werten */
class ArgumentParser {
private:
    int argc_;
    char** argv_;

    void showHelpAndExit();

    //String Input
    std::string input_ = "It's BMW, not 'B M Double You', Bayerische Motorenwerke. Wait what? BAYERISCHE MOTORENWERKE!!!!!!";
    
    // Gaussian Noise
    float snr_ = 5.0f;
    int a_ = 1;

    float p_bsc_ = 0.1f;  // Binary Symmetric
    float p_bec_ = 0.1f;  // Binary Erasure

    //Gilbert-Elliot-Burst
    float errorRateBurst_ = 0.5f;
    float badProb_ = 0.01f;
    float goodProb_ = 0.1f;

public:
    ArgumentParser(int argc, char* argv[]);
    
    void parse();

    // Getter für main
    const std::string& getInput() const { return input_; }
};
