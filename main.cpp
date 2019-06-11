#include "Node.h"
#include "Input.h"
#include <iostream>

int main() {
    int Nodenum = 0, operatenum = 0, evalnum = 0;
    std::cin >> Nodenum;
    for (int i = 1; i <= Nodenum; i++) {  // 构建变量结点
        InputNode();
    }
    std::cin >> operatenum;
    getchar();  // 吸收operatenum后的换行符
    for (int i = 1; i <= operatenum; i++) {  // 构建运算符结点
        Inputoperate();
    }
    std::cin >> evalnum;
    getchar();  // 吸收evalnum后的换行符
    for (int i = 1; i <= evalnum; i++) {  // 进行输出运算操作
        Inputevalnum(i);
    }
    delete_memory();  // 释放内存
    return 0;
}
