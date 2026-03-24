import subprocess


PROGRAM = "../build/ldpc" 
PROBS = [0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4] #Wahrscheinlichkeit für Bit-Flip
CHUNKS = 10000
numberBits = CHUNKS * 512

print("BSC;Hamming;BER") 

with open("results_bsc.csv", "w") as f:
    f.write("BSC;Hamming;BER\n")
    
    for p in PROBS:
        cmd = [PROGRAM, "--bsc", str(p),"--input", "simulate", str(CHUNKS)]
        result = subprocess.run(cmd, capture_output=True)
        output = result.stdout.decode("utf-8", errors="replace")
        
        for line in output.split('\n'):
            if "HAMMING TOTAL:" in line:
                total_hamming = int(line.split(":")[1].strip())
                
                print(f"{p};{total_hamming};{total_hamming/numberBits}")
                f.write(f"{p};{total_hamming};{total_hamming/numberBits}\n")
                f.flush() 

