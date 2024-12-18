#include <iostream>
#include <vector>
#include <chrono>

int linear_search(const std::vector<std::string> &list, const std::string &target, int &iterations)
{
    iterations = 0;
    for (size_t i = 0; i < list.size(); ++i)
    {
        ++iterations;
        if (list[i] == target)
        {
            return i;
        }
    }
    return -1;
}