import subprocess

PROGRAM = "./build/ldpc" 
SNRS = [round(x * 0.5, 1) for x in range(2, 21)] 
CHUNKS = 10000 # 10000 Chunks werden geschickt
numberBits = CHUNKS * 512

print("SNR;Hamming;BER") 

with open("results_awgn.csv", "w") as f:
    f.write("SNR;Hamming;BER\n")
    
    for snr in SNRS:
        cmd = [PROGRAM, "--awgn", str(snr), "1", "--input", "simulate", str(CHUNKS)]    
        result = subprocess.run(cmd, capture_output=True)
        output = result.stdout.decode("utf-8", errors="replace")
        
        # es wird die zahl hinter HAMMING TOTAL gesucht
        for line in output.split('\n'):
            if "HAMMING TOTAL:" in line:
                total_hamming = int(line.split(":")[1].strip())
                
                print(f"{snr};{total_hamming};{total_hamming/numberBits}")
                f.write(f"{snr};{total_hamming};{total_hamming/numberBits}\n")
                f.flush() 