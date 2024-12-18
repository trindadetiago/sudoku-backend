#include <string>
#include <vector>
#include <utility>
#include <ctime>
#include <cstdlib>

struct SkipNode
{
    std::string value;
    int originalIndex; // New field to store the original index
    std::vector<SkipNode *> forward;

    SkipNode(const std::string &val, int index, int level) : value(val), originalIndex(index), forward(level, nullptr) {}
};

class SkipList
{
public:
    SkipList(int maxLevel, float probability) : maxLevel(maxLevel), probability(probability)
    {
        head = new SkipNode("", -1, maxLevel); // -1 for originalIndex as it's a dummy head node
        srand(static_cast<unsigned>(time(nullptr)));
    }

    ~SkipList()
    {
        delete head;
    }

    void insert(const std::pair<std::string, int> &data)
    {
        const std::string &value = data.first;
        int index = data.second;

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
        SkipNode *newNode = new SkipNode(value, index, level);

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
            return current->originalIndex; // Return the original index of the found value
        }
        return -1; // Not found
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
