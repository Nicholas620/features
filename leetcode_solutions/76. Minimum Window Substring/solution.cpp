#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;
class Solution {
public:
    bool is_sub_map(std::map<char, int> &str1, std::map<char, int>& str2) {
        for (auto it = str2.begin(); it != str2.end(); it++) {
            if (it->second > str1[it->first]) return false;
        }
        return true;
    }

    std::string minWindow(std::string& string, std::string& alp) {
        if (alp.size() > string.size() || string.empty() || alp.empty()) return "";

        std::pair<int, int> answer{0, static_cast<int>(string.size())};
        std::map<char, int> alphabet, window;

        for (char c : alp) alphabet[c]++;

        int left = 0;
        for (int right = 0; right < string.size(); right++) {
            window[string[right]]++;
            if (alphabet.contains(string[right])) {
                if (is_sub_map(window, alphabet)) {
                    while (left <= right) {
                        if (right - left < answer.second - answer.first) {
                            answer = {left, right};
                        }

                        if (alphabet[string[left]] < window[string[left]]){
                            window[string[left]]--;
                            left++;
                        } else {
                            break;
                        }
                    }
                }

                std::cout << right;
            }
        }

        if (answer.second == string.size()) return "";

        return string.substr(answer.first, answer.second - answer.first + 1);
    }
};