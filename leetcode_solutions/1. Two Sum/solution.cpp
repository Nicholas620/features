#include <iostream>
#include <vector>
#include <map>

class Solution {
public:
    std::vector<int> twoSum(const std::vector<int>& nums, int target) {
        std::map<int, int> numbers_map;
        std::vector<int> answer;

        for(int i = 0; i < nums.size(); i++){
            int required = target - nums[i];
            if(numbers_map.contains(required)){
                answer.push_back(numbers_map[required]);
                answer.push_back(i);
            }
            numbers_map[nums[i]] = i;
        }

        return answer;
    }
};