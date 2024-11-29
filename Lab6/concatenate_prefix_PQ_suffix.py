import subprocess

bit_to_insert : int = 12352

# List of commands to run in sequence
TERM_COMMANDS = [
    f"head -c {bit_to_insert} a.out > prefix",
    f"tail -c +{bit_to_insert + 128} a.out > suffix",
    "md5collgen -p prefix -o out1.bin out2.bin",
    "tail -c 128 out1.bin > P",
    "tail -c 128 out2.bin > Q",
    "cat prefix P suffix > a1.out",
    "cat prefix Q suffix > a2.out",
    "md5sum a1.out",
    "md5sum a2.out"
]

if __name__ == '__main__':

# Run each command in sequence
    for command in TERM_COMMANDS:
        print(f"executing command: {command}")
        subprocess.run(command, shell=True)
