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
    size_t chunk_ = 0; //Anzahl Chunks
    // Wenn chunks_ != 0 --> erzeuge zufällige chunks (und zwar "chunks_"-Stück)

    // Gaussian Noise
    float snr_ = 5.0f;
    int a_ = 1;

    float p_bsc_ = 0.1f;  // Binary Symmetric
    float p_bec_ = 0.1f;  // Binary Erasure

    //Gilbert-Elliot-Burst
    float errorRateBurst_ = 0.5f;
    float badProb_ = 0.01f;
    float goodProb_ = 0.1f;

    //Ob Noise Modelle beim start augewählt wurden
    bool bsc_ = 0;
    bool gaussian_ = 1; //Gaussian per Default immer enabled KORREKTUR????
    bool erasure_ = 0;
    bool burst_ = 0;

public:
    ArgumentParser(int argc, char* argv[]);
    bool randomMessage = false;
    
    void parse();


    // Getter für main
    const std::string& getInput() const { return input_; }
    size_t getNumberChunks() const { return chunk_; }
    float getAmplitude() const { return a_; }
    float getSNR() const { return snr_; }
    float getBSCProb() const { return p_bsc_; }
    float getBECProb() const { return p_bec_; }
    float getErrorRate() const { return errorRateBurst_; }
    float getBadProb() const { return badProb_; }
    float getGoodProb() const { return goodProb_; }
    bool isBSCEnabled() const { return bsc_; } 
    bool isAWGNEnabled() const { return gaussian_; } 
    bool isErasureEnabled() const { return erasure_; } 
    bool isBurstEnabled() const { return burst_; } 
};
