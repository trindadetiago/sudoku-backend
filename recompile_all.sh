#!/bin/bash
chmod +x $0

# Compile all components
(cd linear_search_files && ./compile_linear_search.sh)
(cd binary_search_files && ./compile_binary_search.sh)
(cd skip_list_files && ./compile_skip_list.sh)
g++ -std=c++17 -o central_search central_search.cpp
