#include "skip_list_search.cpp"
#include <fstream>
#include <sys/resource.h>

long get_memory_usage_kb()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // Memory usage in kilobytes
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file_path>\n";
        return 1;
    }

    std::string input_path = argv[1];
    std::ifstream infile(input_path);
    if (!infile)
    {
        std::cerr << "Error opening file: " << input_path << "\n";
        return 1;
    }

    std::vector<std::string> list;
    std::string line;
    int index = 0;
    while (std::getline(infile, line))
    {
        list.push_back(line);
        index++;
    }

    if (list.empty())
    {
        std::cerr << "Input file is empty.\n";
        return 1;
    }

    std::string target = list.back();
    list.pop_back();

    SkipList skipList(16, 0.5);
    for (size_t i = 0; i < list.size(); ++i)
    {
        skipList.insert({list[i], static_cast<int>(i)});
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
        std::cout << "Value found at original index: " << result << "\n";
    }
    else
    {
        std::cout << "Value not found in the list.\n";
    }
    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "Time taken: " << duration.count() << " microseconds\n";
    std::cout << "Memory used: " << (memory_after - memory_before) << " KB\n\n";

    return 0;
}
