#include "argument_parser.h"

#include <iostream>

ArgumentParser::ArgumentParser (int argc, char* argv[]) 
: argc_(argc), argv_(argv){};

void ArgumentParser::showHelpAndExit(){
  std::cout << "Usage: ./ldpc [PARAMETERS]\n\n"
              << "Parameters:\n"
              << "  --input [msg <string> | simulate <int>]   \n"
              << "           msg <string>        Start with a custom input string (Recommended)\n"
              << "           simulate <int>      Start with <int>-number of chunks (randomly filled)\n"
              << "  --bsc <p>             Binary Symmetric Channel\n"
              << "                        <p>: Bit-flip probability [0.0 - 1.0]\n\n"
              << "  --awgn <snr> <a>    Additive White Gaussian Noise\n"
              << "                        <snr>: Signal-to-Noise Ratio [dB]\n"
              << "                        <a>: Noise amplitude\n\n"
              << "  --bec <p>             Binary Erasure Channel\n"
              << "                        <p>: Erasure probability\n\n"
              << "  --burst <e> <b> <g>   Gilbert-Elliott Burst Error Model.\n"
              << "                        <e>: Erasure rate in BAD state\n"
              << "                        <b>: Transition probability GOOD->BAD\n"
              << "                        <g>: Transition probability BAD->GOOD\n\n"
              << "  --help                Show this help message and exit\n"
              << std::endl;
            std::exit(EXIT_SUCCESS);

}

void ArgumentParser::parse(){
      for (int i = 1; i < argc_; ++i) {
        if (strcmp( argv_[i], "--help") == 0) {
            showHelpAndExit();
        } else if (strcmp( argv_[i], "--input")==0) {
          if (strcmp( argv_[i+1], "msg") ==0  ){
            this->input_ = argv_[i+2];
            std::cout << "[INFO] Message: " << input_ << "\n" << std::endl;
          } else if (strcmp( argv_[i+1], "simulate") ==0  ) {
            this->chunk_ = std::stoi(argv_[i+2]);
            randomMessage = true; //message soll random gefüllt werden (für simulation)

          }

        }else if 
          (strcmp( argv_[i], "--awgn") == 0 && (i + 2 < argc_)){
            gaussian_ = true;
          
                try {
                  this->snr_ = std::stof(argv_[++i]);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "ERROR: --awgn <snr> <a>" << std::endl;
                    std::cerr << "               ^ snr should be a float" << std::endl;
                    std::exit(EXIT_FAILURE);
                }

                try {
                  this->a_ = std::stoi(argv_[++i]);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "ERROR: --awgn <snr> <a>" << std::endl;
                    std::cerr << "                     ^ amplitude should be an integer" << std::endl;
                    std::exit(EXIT_FAILURE);
                }

                std::cout << "SNR = " << snr_ << "[dB]" << std::endl;
                std::cout << "Amplitude  = " << a_ << std::endl;

            


        } else if (strcmp( argv_[i], "--bsc") == 0 && (i + 1 < argc_)){
                bsc_ = true;
          
                try {
                  this->p_bsc_ = std::stof(argv_[++i]);

                  if (p_bsc_ < 0.0f || p_bsc_ > 1.0f) {
                      std::cerr << "ERROR: Bit Flip Probability should lie between 0.0 and 1.0. Input: " << p_bsc_ << std::endl;
                      std::exit(EXIT_FAILURE); 
        }

                } catch (const std::invalid_argument& e) {
                    std::cerr << "ERROR: --bsc <p>" << std::endl;
                    std::cerr << "              ^ p should be a float in range [0.0 - 1.0]" << std::endl;
                    std::exit(EXIT_FAILURE);
        }
      } else if (strcmp( argv_[i], "--bec") == 0 && (i + 1 < argc_)){
        erasure_ = true;
          
                try {
                  this->p_bec_ = std::stof(argv_[++i]);

                  if (p_bec_ < 0.0f || p_bec_ > 1.0f) {
                      std::cerr << "ERROR: Bit Erasure Probability should lie between 0.0 and 1.0. Input: " << p_bec_ << std::endl;
                      std::exit(EXIT_FAILURE); 
        }

                } catch (const std::invalid_argument& e) {
                    std::cerr << "ERROR: --bec <p>" << std::endl;
                    std::cerr << "              ^ p should be a float in range [0.0 - 1.0]" << std::endl;
                    std::exit(EXIT_FAILURE);
      }
      }

      //--burst <e> <b> <g>   Gilbert-Elliott Burst Error Model
      else if (strcmp( argv_[i], "--burst") == 0 && (i + 3 < argc_)){ 
        burst_ = true;

                  // Wahrscheinlichkeit für den Erasure in BAD State
                  const std::string burstMsg = "ERROR: --burst <e> <b> <g>\n"
                                                "                       ^ g should be a float in range [0.0 - 1.0]";
                  try {
                  this->errorRateBurst_ = std::stof(argv_[++i]);

                  if (errorRateBurst_ < 0.0f || errorRateBurst_ > 1.0f) {
                      std::cerr <<burstMsg << " (Invalid range)" << std::endl;
                      std::exit(EXIT_FAILURE); 
                   }
                  } catch (const std::invalid_argument& e) {
                    std::cerr <<burstMsg << " (Invalid datatype)" << std::endl;
                    std::exit(EXIT_FAILURE);
                  }

                   // Wahrscheinlichkeit für den Wechsel von GOOD --> BAD   
                  const std::string badProbMsg = "ERROR: --burst <e> <b> <g>\n"
                                                 "                    ^ b should be a float in range [0.0 - 1.0]";
                  try {
                  this->badProb_ = std::stof(argv_[++i]);

                  if (badProb_ < 0.0f || badProb_ > 1.0f) {
                    std::cerr << badProbMsg << " (Invalid range)" << std::endl;
                    std::exit(EXIT_FAILURE); 
                   }
                  } catch (const std::invalid_argument& e) {
                    std::cerr << badProbMsg << " (Invalid datatype)"  << std::endl;
                    std::exit(EXIT_FAILURE);
                  }

                  // Wahrscheinlichkeit für den Wechsel von BAD --> GOOD      
                  const std::string goodProbMsg = "ERROR: --burst <e> <b> <g>\n"
                                                  "                        ^ g should be a float in range [0.0 - 1.0]";
                  try {
                  this->goodProb_ = std::stof(argv_[++i]);
                  if (goodProb_ < 0.0f || goodProb_ > 1.0f) {
                      std::cerr << goodProbMsg << " (Invalid range)"<< std::endl;

                      std::exit(EXIT_FAILURE); 
                   }
                  } catch (const std::invalid_argument& e) {
                      std::cerr << goodProbMsg << " (Invalid datatype)"<< std::endl;
                    std::exit(EXIT_FAILURE);
                  }


      }
      }
}

