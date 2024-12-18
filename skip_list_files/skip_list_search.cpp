#include "skip_list.cpp"
#include <chrono>
#include <iostream>
#include <vector>

int skip_list_search(SkipList &skipList, const std::string &target, int &iterations)
{
    return skipList.search(target, iterations);
}