#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <sys/resource.h> // For memory usage

// Include the headers for the search algorithms
#include "linear_search_files/linear_search.cpp"
#include "binary_search_files/binary_search.cpp"
#include "skip_list_files/skip_list.cpp"

long get_memory_usage_kb()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // Memory usage in kilobytes
}

void execute_linear_search(const std::vector<std::string> &data, const std::string &target)
{
    int iterations = 0;
    long memory_before = get_memory_usage_kb();
    auto start = std::chrono::high_resolution_clock::now();
    int index = linear_search(data, target, iterations);
    auto end = std::chrono::high_resolution_clock::now();
    long memory_after = get_memory_usage_kb();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if (index != -1)
    {
        std::cout << "Linear Search:\n Value found at index: " << index << "\n";
    }
    else
    {
        std::cout << "Linear Search:\n Value not found in the list.\n";
    }
    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "Time taken: " << duration.count() << " microseconds\n";
    std::cout << "Memory used: " << (memory_after - memory_before) << " KB\n\n";
}

void execute_binary_search(const std::vector<std::string> &data, const std::string &target)
{
    std::vector<std::string> sorted_data = data;
    std::sort(sorted_data.begin(), sorted_data.end());

    int iterations = 0;
    long memory_before = get_memory_usage_kb();
    auto start = std::chrono::high_resolution_clock::now();
    int index = binary_search(sorted_data, target, iterations);
    auto end = std::chrono::high_resolution_clock::now();
    long memory_after = get_memory_usage_kb();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if (index != -1)
    {
        std::cout << "Binary Search:\n Value found at index: " << index << "\n";
    }
    else
    {
        std::cout << "Binary Search:\n Value not found in the list.\n";
    }
    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "Time taken: " << duration.count() << " microseconds\n";
    std::cout << "Memory used: " << (memory_after - memory_before) << " KB\n\n";
}

void execute_skip_list_search(const std::vector<std::string> &data, const std::string &target)
{
    SkipList skipList(16, 0.5);
    for (const auto &val : data)
    {
        skipList.insert(val);
    }

    int iterations = 0;
    long memory_before = get_memory_usage_kb();
    auto start = std::chrono::high_resolution_clock::now();
    int result = skipList.search(target, iterations);
    auto end = std::chrono::high_resolution_clock::now();
    long memory_after = get_memory_usage_kb();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if (result != -1)
    {
        std::cout << "Skip List Search:\n Value found.\n";
    }
    else
    {
        std::cout << "Skip List Search:\n Value not found in the list.\n";
    }
    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "Time taken: " << duration.count() << " microseconds\n";
    std::cout << "Memory used: " << (memory_after - memory_before) << " KB\n\n";
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file_path> <search_types_comma_separated>\n";
        return 1;
    }

    std::string input_file = argv[1];
    std::string types = argv[2];

    std::ifstream infile(input_file);
    if (!infile)
    {
        std::cerr << "Error opening file: " << input_file << "\n";
        return 1;
    }

    std::vector<std::string> data;
    std::string line;
    while (std::getline(infile, line))
    {
        data.push_back(line);
    }

    if (data.empty())
    {
        std::cerr << "Input file is empty.\n";
        return 1;
    }

    std::string target = data.back();
    data.pop_back();

    std::vector<std::string> search_types;
    size_t pos = 0;
    while ((pos = types.find(',')) != std::string::npos)
    {
        search_types.push_back(types.substr(0, pos));
        types.erase(0, pos + 1);
    }
    search_types.push_back(types);

    for (const auto &type : search_types)
    {
        if (type == "linear")
        {
            execute_linear_search(data, target);
        }
        else if (type == "binary")
        {
            execute_binary_search(data, target);
        }
        else if (type == "skip_list")
        {
            execute_skip_list_search(data, target);
        }
        else
        {
            std::cerr << "Unknown search type: " << type << "\n";
        }
    }

    return 0;
}