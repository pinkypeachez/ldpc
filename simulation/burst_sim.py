import subprocess

PROGRAM = "../build/ldpc"
CHUNKS = 10000
numberBits = CHUNKS * 512

E          = 1.0
P_BG       = 0.1
P_GB_SWEEP = [0.001, 0.005, 0.01, 0.02, 0.05, 0.1, 0.2, 0.3, 0.5]

print("p_GB;Hamming;BER")

with open("results_burst.csv", "w") as f:
    f.write("p_GB;Hamming;BER\n")

    for p_gb in P_GB_SWEEP:
        cmd = [PROGRAM, "--burst", str(E), str(p_gb), str(P_BG),
               "--input", "simulate", str(CHUNKS)]
        result = subprocess.run(cmd, capture_output=True)
        output = result.stdout.decode("utf-8", errors="replace")

        for line in output.split('\n'):
            if "HAMMING TOTAL:" in line:
                total_hamming = int(line.split(":")[1].strip())

                print(f"{p_gb};{total_hamming};{total_hamming/numberBits}")
                f.write(f"{p_gb};{total_hamming};{total_hamming/numberBits}\n")
                f.flush()
