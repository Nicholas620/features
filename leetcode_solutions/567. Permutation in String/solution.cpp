#include <iostream>
#include <string>
#include <map>

class Solution {
public:
    bool checkInclusion(const std::string& s1, const std::string& s2) {
        if(s1.size() > s2.size()) return false;
        std::map<char, int> window, alphabet;

        for(int i = 0; i < s1.size(); i++){
            window[s2[i]] ++;
            alphabet[s1[i]] ++;
        }

        for(int i = s1.size(); i <= s2.size(); i++){
            if(window == alphabet) return true;

            window[s2[i]]++;
            window[s2[i - s1.size()]]--;

            if(window[s2[i - s1.size()]] == 0) window.erase(s2[i - s1.size()]);
        }

        return false;
    }
};