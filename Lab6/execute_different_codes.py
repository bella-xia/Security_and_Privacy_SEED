import subprocess

start_of_X_str = 12320
start_of_Y_str = 12544

bit_to_insert : int = 12352

# List of commands to run in sequence
TERM_COMMANDS = [
    f"head -c {bit_to_insert} a.out > prefix",
    f"tail -c +{bit_to_insert + 128  + 1} a.out > suffix",
    "md5collgen -p prefix -o out1.bin out2.bin",
    "tail -c 128 out1.bin > P",
    "tail -c 128 out2.bin > Q",
    f"head -c {start_of_Y_str - 128 - start_of_X_str} suffix > suffix_1",
    f"tail -c +{start_of_Y_str - start_of_X_str + 1} suffix > suffix_2",
    "cat prefix P suffix_1 P suffix_2 > a1.out",
    "cat prefix Q suffix_1 P suffix_2 > a2.out",
    "md5sum a1.out",
    "md5sum a2.out"
]

if __name__ == '__main__':

# Run each command in sequence
    for command in TERM_COMMANDS:
        print(f"executing command: {command}")
        subprocess.run(command, shell=True)
