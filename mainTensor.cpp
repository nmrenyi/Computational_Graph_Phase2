// 本文件为Tensor运算的入口
#include"_Tensor.h"
#include<iostream>
#include<map>
#include<string>
#include<sstream>
#include<cstdlib>
#include<algorithm>

// 一些函数声明
void Inputoperate(std::map<std::string, Tensor*>&);
void InputNode(std::map<std::string, Tensor*>&);
void Inputevalnum(int, std::map<std::string, Tensor*>&);

int main() {
    std::map<std::string, Tensor*> save;
    int Nodenum = 0, operatenum = 0, evalnum = 0;
    std::cin >> Nodenum;
    for (int i = 1; i <= Nodenum; i++) {  // 构建变量结点
        InputNode(save);
    }
    std::cin >> operatenum;
    getchar();  // 吸收operatenum后的换行符
    for (int i = 1; i <= operatenum; i++) {  // 构建运算符结点
        Inputoperate(save);
    }
    std::cin >> evalnum;
    getchar();  // 吸收evalnum后的换行符
    for (int i = 1; i <= evalnum; i++) {  // 进行输出运算操作
        Inputevalnum(i, save);
    }
    return 0;
}
<<<<<<< HEAD

int getDataNum(std::vector<int>dim) {
    int tmp = 1;
    for (auto x : dim) {
        tmp *= x;
    }
    return tmp;
}
void InputNode(std::map<std::string, Tensor*>& save) {
    std::string name;
    char type;
    int dimNum = 0;
    std::vector<int>dim;
    std::cin >> name >> type >> dimNum;
    for (int i = 0; i < dimNum; i++) {
        int tmp = 0;
        std::cin >> tmp;
        dim.push_back(tmp);
    }

    // 查找之前是否存在同名结点(指最开始的C、P、V类型)
    // 如果存在，将其push_back进save_for_delete以便后来删除而防止内存泄漏
    // if (save.find(name) != save.end()) {
    //     save_for_delete.push_back(save[name]);
    // }

    switch (type) {  //判断输入的变量的类型
        case 'P': {
            Tensor* Newplace = new TensorPlaceholder(dim);
            save[name] = Newplace;
        }
        break;
        case 'C': {
            std::vector<double>data;
            int dataNum = getDataNum(dim);
            for (int i = 0; i < dataNum; i++) {
                double tmp = 0;
                std::cin >> tmp;
                data.push_back(tmp);
            }
            Tensor* Newconstant = new TensorConstant(dim, data);
            save[name] = Newconstant;
        }
        break;
        case 'V': {
            std::vector<double>data;
            int dataNum = getDataNum(dim);
            for (int i = 0; i < dataNum; i++) {
                double tmp = 0;
                std::cin >> tmp;
                data.push_back(tmp);
            }
            Tensor* newVariable = new TensorVariable(dim, data);
            save[name] = newVariable;
        }
        break;
    }
}


void Inputoperate(std::map<std::string, Tensor*>& save) {
    std::string str;
    std::string tmp;
    std::vector <std::string> buffer;  // 用sstream存储当前输入的关键信息
    getline(std::cin, str);
    std::stringstream ss(str);
    while (ss >> tmp) { buffer.push_back(tmp); }

    // 查找之前是否存在同名结点（指单目、双目等运算符类型）
    // 如果存在，将其push_back进save_for_delete以便后来删除而防止内存泄漏
    // if (save.find(buffer[0]) != save.end()) {
    //     save_for_delete.push_back(save[buffer[0]]);
    // }

    if (buffer.size() == 4) {  // a = SIN x 单目运算符
        Tensor* single = new TensorSingleOperation(buffer[3], buffer[2], save);
        save[buffer[0]] = single;
    }
    else if (buffer.size() == 5) {  // a = x + y 双目运算符
        Tensor* binary =
        new TensorBinaryOperation(buffer[2], buffer[4], buffer[3], save);
        save[buffer[0]] = binary;
    } else if (buffer.size() == 6) { // b = reshape a m n
        Tensor* reshape = new TensorReshapeOperation(buffer[4], buffer[5], buffer[3], save);

        
        
    } else {
        std::cout << "invalid input type" << std::endl;
    }
}
=======
>>>>>>> ry
