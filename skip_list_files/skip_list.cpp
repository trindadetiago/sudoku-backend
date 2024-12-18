#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

struct SkipNode
{
    std::string value;
    std::vector<SkipNode *> forward;

    SkipNode(const std::string &val, int level) : value(val), forward(level, nullptr) {}
};

class SkipList
{
public:
    SkipList(int maxLevel, float probability) : maxLevel(maxLevel), probability(probability)
    {
        head = new SkipNode("", maxLevel);
        srand(static_cast<unsigned>(time(nullptr)));
    }

    ~SkipList()
    {
        delete head;
    }

    void insert(const std::string &value)
    {
        std::vector<SkipNode *> update(maxLevel, nullptr);
        SkipNode *current = head;

        for (int i = maxLevel - 1; i >= 0; --i)
        {
            while (current->forward[i] && current->forward[i]->value < value)
            {
                current = current->forward[i];
            }
            update[i] = current;
        }

        int level = randomLevel();
        SkipNode *newNode = new SkipNode(value, level);

        for (int i = 0; i < level; ++i)
        {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }

    int search(const std::string &value, int &iterations)
    {
        iterations = 0;
        SkipNode *current = head;
        for (int i = maxLevel - 1; i >= 0; --i)
        {
            while (current->forward[i] && current->forward[i]->value < value)
            {
                current = current->forward[i];
                ++iterations;
            }
        }

        current = current->forward[0];
        if (current && current->value == value)
        {
            return iterations;
        }
        return -1;
    }

private:
    int maxLevel;
    float probability;
    SkipNode *head;

    int randomLevel()
    {
        int level = 1;
        while (((float)rand() / RAND_MAX) < probability && level < maxLevel)
        {
            ++level;
        }
        return level;
    }
};