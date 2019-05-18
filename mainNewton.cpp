#include "Node.h"
#include <iostream>
#include <vector>
#include <algorithm>
void newtonMethod(std::vector<double> equation);
int main() {
    int m = 0;  // m个方程
    std::cin >> m;
    std::vector<std::vector<double>>equation;
    for (int i = 0; i < m; i++) {
        std::vector<double> tmp_equation;
        int n = 0;  // 方程最高次数n
        std::cin >> n;
        for (int j = 0; j <= n + 1; j++) {
            int tmp = 0;
            std::cin >> tmp;
            tmp_equation.push_back(tmp);
        }
        tmp_equation.push_back(n);
        std::reverse(tmp_equation.begin(), tmp_equation.end() - 1);  // 调换系数的储存顺序，方便按顺序访问
        equation.push_back(tmp_equation);         
    }
    for (auto x : equation) {
        newtonMethod(x);
    }
    return 0;
}
