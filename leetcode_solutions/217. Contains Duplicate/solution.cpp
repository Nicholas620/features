#include <iostream>
#include <vector>
#include <set>

class Solution {
public:
    bool containsDuplicate(const std::vector<int>& nums) {
        std::set<int> unique_numbers;

        for(auto num: nums){
            if(unique_numbers.contains(num)) return true;

            unique_numbers.insert(num);
        }

        return false;
    }
};