// 本文件为Tensor运算的入口
#include"_Tensor.h"
#include<iostream>
#include<map>
#include<string>
#include<sstream>
#include<cstdlib>
#include<algorithm>

// 一些函数声明
void inputOperate(std::map<std::string, Tensor*>&);
void inputNode(std::map<std::string, Tensor*>&);
void inputEvalNum(int, std::map<std::string, Tensor*>&);

int main() {
    std::map<std::string, Tensor*> save;
    int Nodenum = 0, operatenum = 0, evalnum = 0;
    std::cin >> Nodenum;
    for (int i = 1; i <= Nodenum; i++) {  // 构建变量结点
        inputNode(save);
    }
    std::cin >> operatenum;
    getchar();  // 吸收operatenum后的换行符
    for (int i = 1; i <= operatenum; i++) {  // 构建运算符结点
        inputOperate(save);
    }
    std::cin >> evalnum;
    getchar();  // 吸收evalnum后的换行符
    for (int i = 1; i <= evalnum; i++) {  // 进行输出运算操作
        inputEvalNum(i, save);
    }
    return 0;
}
