#include <iostream>
#include <vector>
#include <map>

class Solution {
public:
    int totalFruit(const std::vector<int>& fruits) {
        int count = 0, left = 0, right = 0;
        std::map<int, int> map;

        for(; right < fruits.size(); right++){
            if(map.size() < 2 || (map.size() == 2 && map.contains(fruits[right]))){
                map[fruits[right]]++;
                continue;
            }

            count = std::max(right - left, count);

            while(map.size() >= 2 && right > left){
                map[fruits[left]]--;
                if(map[fruits[left]] == 0) map.erase(fruits[left]);
                left++;
            }

            map[fruits[right]]++;
        }
        count = std::max(right - left, count);

        return count;
    }
};