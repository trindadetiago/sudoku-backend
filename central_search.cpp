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
        std::cout << "Linear Search:\nValue found at index: " << index << "\n";
    }
    else
    {
        std::cout << "Linear Search:\nValue not found in the list.\n";
    }
    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "Time taken: " << duration.count() << " microseconds\n";
    std::cout << "Memory used: " << (memory_after - memory_before) << " KB\n\n";
}

void execute_binary_search(const std::vector<std::string> &data, const std::string &target)
{
    // Store original indices alongside the values
    std::vector<std::pair<std::string, int>> indexed_data;
    for (size_t i = 0; i < data.size(); ++i)
    {
        indexed_data.emplace_back(data[i], i);
    }

    // Sort the data by value
    std::sort(indexed_data.begin(), indexed_data.end());

    int iterations = 0;
    long memory_before = get_memory_usage_kb();
    auto start = std::chrono::high_resolution_clock::now();

    // Perform binary search
    int left = 0, right = indexed_data.size() - 1, found_index = -1;
    while (left <= right)
    {
        ++iterations;
        int mid = left + (right - left) / 2;
        if (indexed_data[mid].first == target)
        {
            found_index = indexed_data[mid].second; // Retrieve the original index
            break;
        }
        if (indexed_data[mid].first < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    long memory_after = get_memory_usage_kb();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if (found_index != -1)
    {
        std::cout << "Binary Search:\nValue found at original index: " << found_index << "\n";
    }
    else
    {
        std::cout << "Binary Search:\nValue not found in the list.\n";
    }
    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "Time taken: " << duration.count() << " microseconds\n";
    std::cout << "Memory used: " << (memory_after - memory_before) << " KB\n\n";
}

void execute_skip_list_search(const std::vector<std::string> &data, const std::string &target)
{
    SkipList skipList(16, 0.5);

    // Insert values along with their original indices
    for (size_t i = 0; i < data.size(); ++i)
    {
        skipList.insert(std::make_pair(data[i], i));
    }

    int iterations = 0;
    long memory_before = get_memory_usage_kb();
    auto start = std::chrono::high_resolution_clock::now();

    // Search in SkipList
    auto result = skipList.search(target, iterations);
    auto end = std::chrono::high_resolution_clock::now();
    long memory_after = get_memory_usage_kb();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if (result != -1)
    {
        std::cout << "Skip List Search:\nValue found at original index: " << result << "\n";
    }
    else
    {
        std::cout << "Skip List Search:\nValue not found in the list.\n";
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