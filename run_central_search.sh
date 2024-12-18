#!/bin/bash
chmod +x $0

# Predefined input file and search types
input_file="./test_input.txt"
search_types="linear,binary,skip_list"

# Compile everything
./recompile_all.sh

# Run the central search program
./central_search "$input_file" "$search_types"