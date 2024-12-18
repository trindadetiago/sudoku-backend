#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

int binary_search(const std::vector<std::string> &list, const std::string &target, int &iterations)
{
    iterations = 0;
    int left = 0, right = list.size() - 1;
    while (left <= right)
    {
        ++iterations;
        int mid = left + (right - left) / 2;
        if (list[mid] == target)
        {
            return mid;
        }
        else if (list[mid] < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return -1;
}