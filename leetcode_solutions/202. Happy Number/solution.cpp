#include <cmath>
#include <iostream>
#include <map>

class Solution {
public:
    bool isHappy(int n) {
        if(n == 1) return true;
        std::map<int, int> met;
        while(met[n] == 0){
            met[n]++;
            int local_sum = 0;
            while(n > 9){
                int reminder = n % 10;
                n /= 10;
                local_sum += pow(reminder, 2);
            }
            n = pow(n, 2) + local_sum;

            if(n == 1) return true;
        }

        return false;
    }
};