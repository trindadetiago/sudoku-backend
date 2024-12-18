## Project Structure

# Directory: linear_search_files
- `linear_search.cpp`: Contains the implementation of linear search.
- `linear_search_main.cpp`: Reads input file and calls the linear search function.
- `compile_linear_search.sh`: Compiles both `.cpp` files into an executable `linear_search`.
- `run_linear_search.sh`: Runs the `linear_search` executable with a provided input file path.

# Directory: binary_search_files
- `binary_search.cpp`: Contains the implementation of binary search.
- `binary_search_main.cpp`: Reads input file and calls the binary search function.
- `compile_binary_search.sh`: Compiles both `.cpp` files into an executable `binary_search`.
- `run_binary_search.sh`: Runs the `binary_search` executable with a provided input file path.

# Directory: skip_list_files
- `skip_list_search.cpp`: Contains the implementation of skip list search.
- `skip_list_search_main.cpp`: Reads input file and calls the skip list search function.
- `compile_skip_list.sh`: Compiles both `.cpp` files into an executable `skip_list_search`.
- `run_skip_list.sh`: Runs the `skip_list_search` executable with a provided input file path.

# Root Directory
- `central_search.cpp`: Central coordinator that:
  - Reads input file path.
  - Reads the list of types to execute.
  - Invokes corresponding search executables.
  - Aggregates and prints results.
- `recompile_all.sh`: Recompiles all `.cpp` files in the project.
- `run_central_search.sh`: Runs the `central_search` executable, defining the input file path and search types.

## Shell Scripts Permissions
All `.sh` scripts include `chmod +x` to set executable permissions automatically.

Remember to run `chmod +x *.sh`at the root.

---

# Example File Content

## linear_search.cpp
```cpp
#include <iostream>
#include <vector>
#include <chrono>

int linear_search(const std::vector<std::string>& list, const std::string& target, int& iterations) {
    iterations = 0;
    for (size_t i = 0; i < list.size(); ++i) {
        ++iterations;
        if (list[i] == target) {
            return i;
        }
    }
    return -1;
}
```

## linear_search_main.cpp
```cpp
#include "linear_search.cpp"
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file_path>\n";
        return 1;
    }

    std::string input_path = argv[1];
    std::ifstream infile(input_path);
    if (!infile) {
        std::cerr << "Error opening file: " << input_path << "\n";
        return 1;
    }

    std::vector<std::string> list;
    std::string line;
    while (std::getline(infile, line)) {
        list.push_back(line);
    }

    if (list.empty()) {
        std::cerr << "Input file is empty.\n";
        return 1;
    }

    std::string target = list.back();
    list.pop_back();

    int iterations = 0;
    auto start = std::chrono::high_resolution_clock::now();
    int index = linear_search(list, target, iterations);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if (index != -1) {
        std::cout << "Value found at index: " << index << "\n";
    } else {
        std::cout << "Value not found in the list.\n";
    }
    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "Time taken: " << duration.count() << " microseconds\n";

    return 0;
}
```

## compile_linear_search.sh
```bash
#!/bin/bash
chmod +x $0

g++ -std=c++17 -o linear_search linear_search_main.cpp
```

## run_linear_search.sh
```bash
#!/bin/bash
chmod +x $0

# Predefined input file path
input_file="input.txt"

# Compile the linear search program
./compile_linear_search.sh

# Run the linear search program with the predefined input file
./linear_search "$input_file"
```

---

This pattern will be followed for `binary_search_files` and `skip_list_files`. Root-level scripts will integrate everything. Let me know if you want adjustments or all files expanded!
