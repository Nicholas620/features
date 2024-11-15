#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;
class Solution {
public:
    int lengthOfLongestSubstring(const string& s) {
        int left = 0, right = 0;
        std::set<char> unique_elements;
        std::pair<int, int> answer(0, 0);

        for(; right < s.size(); right++){
            if(!unique_elements.contains(s[right])){
                unique_elements.insert(s[right]);
                continue;
            }

            if(answer.second - answer.first < right - left) answer = {left, right};

            while(s[left] != s[right] && left < right){
                unique_elements.erase(s[left]);
                left++;
            }
            if(left != right){
                left++;
            }
        }

        if(answer.second - answer.first < right - left) answer = {left, right};

        return answer.second - answer.first;
    }
};