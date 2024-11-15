#include <iostream>
#include <string>
#include <map>

class Solution {
public:
    int subarraySum(std::vector<int>& nums, int k) {
        std::map<int, int> prefix_sums;
        prefix_sums[0] = 1;
        int local_sum = 0, answer = 0;
        for(int i = 0; i < nums.size(); i++){
            local_sum += nums[i];

            auto it = prefix_sums.find(local_sum - k);
            if(it != prefix_sums.end()) answer += it -> second;
            prefix_sums[local_sum]++;
        }

        return answer;
    }
};