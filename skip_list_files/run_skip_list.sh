#!/bin/bash
chmod +x $0

# Predefined input file path
input_file="../test_input.txt"

# Compile the skip list program
./compile_skip_list.sh

# Run the skip list program with the predefined input file
./skip_list_search "$input_file"