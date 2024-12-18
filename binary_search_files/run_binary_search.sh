#!/bin/bash
chmod +x $0

# Predefined input file path
input_file="../test_input.txt"

# Compile the binary search program
./compile_binary_search.sh

# Run the binary search program with the predefined input file
./binary_search "$input_file"