#include <iostream>
#include <vector>

class Solution {
public:
    void moveZeroes(const std::vector<int>& nums) {
        int start = 0;
        for(int i = 0; i < nums.size(); i++) if(nums[i] != 0) nums[start++] = nums[i];
        for(; start < nums.size(); start++) nums[start] = 0;
    }
};