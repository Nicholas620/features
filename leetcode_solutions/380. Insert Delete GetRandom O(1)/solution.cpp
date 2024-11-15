#include <random>
#include <iostream>
#include <map>
#include <vector>

class RandomizedSet {
private:
    std::map<int, int> set_indexes;
    std::vector<int> set;

public:
    RandomizedSet() = default;

    bool insert(int val) {
        if(set_indexes.contains(val)) return false;

        set_indexes[val] = set.size();
        set.push_back(val);
        return true;
    }

    bool remove(int val) {
        if(!set_indexes.contains(val)) return false;

        int index = set_indexes[val];
        set_indexes[set[set.size() - 1]] = index;
        set_indexes.erase(val);
        set[index] = set[set.size() - 1];
        set.pop_back();

        return true;
    }

    int getRandom() {
        int rand_index = std::rand() % set.size();

        return set[rand_index];
    }
};
