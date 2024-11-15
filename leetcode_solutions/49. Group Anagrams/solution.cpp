#include <iostream>
#include <string>
#include <vector>
#include <map>

class Solution {
public:
    vector<vector<std::string>> groupAnagrams(const std::vector<std::string>& strs) {
        std::map<std::string, std::vector<std::string>> anag;
        std::vector<std::vector<std::string>> answer;

        for(const auto& string: strs){
            std::string local_copy = string;
            std::sort(local_copy.begin(), local_copy.end());
            anag[local_copy].push_back(string);
        }

        for(const auto& group: anag){
            answer.push_back(group.second);
        }

        return answer;
    }
};