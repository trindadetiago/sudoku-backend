#!/bin/bash
chmod +x $0

# Predefined input file path
input_file="../test_input.txt"

# Compile the linear search program
./compile_linear_search.sh

# Run the linear search program with the predefined input file
./linear_search "$input_file"