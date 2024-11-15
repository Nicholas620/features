#include <iostream>
#include <vector>
#include <map>

class Solution {
public:
    int at_most_k(const std::vector<int>& nums, int k){
        int count = 0, left = 0;
        std::map<int, int> window;

        for(int right = 0; right < nums.size(); right++){
            window[nums[right]] ++;

            while (window.size() > k){
                window[nums[left]]--;
                if(window[nums[left]] == 0) window.erase(nums[left]);
                left++;
            }

            count += right - left + 1;
        }

        return count;
    }

    int subarraysWithKDistinct(const std::vector<int>& nums, int k) {
        return at_most_k(nums, k) - at_most_k(nums, k - 1);
    }
};