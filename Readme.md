## Compile:
mkdir build
cd build
make

## Start the programm
**Quick start (example):**
./ldpc --bsc 0.1 --input msg "Try using the ascii text for intuitive understanding of debug output" 

--> will result in starting LDPC with custom message and Binary Symmetric Channel (with 0.1 Bit-Flip Probability)

For help use the --help flag:
./ldpc --help

IMPORTANT! SETTING NO FLAGS FOR NOISE RESULT IN NOISELESS TRANSMITION! **YOU MUST SET FLAGS FOR ADDING NOISE!**

Usage: ./ldpc [PARAMETERS]
     "Parameters:
        --input [msg <string> | simulate <int>]
                msg <string>        Start with a custom input string (Recommended)
                simulate <int>      Start with <int>-number of chunks (randomly filled)
        --bsc <p>             Binary Symmetric Channel
              <p>: Bit-flip probability [0.0 - 1.0]
        --awgn <snr> <a>    Additive White Gaussian Noise
               <snr>: Signal-to-Noise Ratio [dB]
               <a>: Noise amplitude
        --bec <p>             Binary Erasure Channel
              <p>: Erasure probability
        --burst <e> <b> <g>   Gilbert-Elliott Burst Error Model
              <e>: Erasure rate in BAD state
              <b>: Transition probability GOOD->BAD
              <g>: Transition probability BAD->GOOD
        --help                Show this help message and exit




       

Quellen:
1. Chapter 15 [Low-Density Parity-Check Codes (Pages: 634-679)]
https://onlinelibrary.wiley.com/doi/book/10.1002/0471739219

2. Gaussian Channel, Modulation, BPSK
SNR: https://youtu.be/UOLRP52oOPI?si=IVXSZQ5tEK4O2cgr&t=614


- Interface: https://www.youtube.com/watch?v=8USgOF7x0hM
