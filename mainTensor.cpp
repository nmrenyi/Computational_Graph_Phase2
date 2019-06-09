// 本文件为Tensor运算的入口
#include"Tensor.h"
#include<iostream>
#include<map>
#include<string>
#include<sstream>
#include<cstdlib>
#include<algorithm>


void Inputoperate(std::map<std::string, Tensor*>& save);
void InputNode(std::map<std::string, Tensor*>& save);

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
    for (int i = 1; i <= evalnum; i++) {  //进行输出运算操作
        Inputevalnum(i);
    }
    return 0;
}

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

std::pair<bool, std::vector<int>> checkBroadcast(Tensor* ptr1, Tensor* ptr2) {
    std::vector<int> dim1 = ptr1->getDim();
    std::vector<int> dim2 = ptr2->getDim();
    std::vector<int>tmp;  // 储存加节点的维数信息
    int dimMin = std::min(dim1.size(), dim2.size());
    for (int i = 1; i <= dimMin; i++) {
        int length1 = *(dim1.end() - i);
        int length2 = *(dim2.end() - i);
        tmp.push_back(std::max(length1, length2));
        if (length1 != length2 && length1 != 1 && length2 != 1)
            return std::make_pair(false, tmp);
    }
    std::reverse(tmp.begin(), tmp.end());
    return std::make_pair(true, tmp);
}

void Inputoperate(std::map<std::string, Tensor*>& save) {
    std::string str;
    std::string tmp;
    std::vector <std::string> buffer;  // 用sstream存储当前输入的关键信息
    getline(std::cin, str);
    std::stringstream ss(str);
    while (ss >> tmp) { buffer.push_back(tmp); }

    if (buffer.size() == 4) {  // a = SIN x 单目运算符
        Tensor* single = new TensorSingleOperation(buffer[3], buffer[2], save);
        save[buffer[0]] = single;
    }
    else if (buffer.size() == 5) {  // a = x + y 双目运算符
        std::pair<bool, vector<int>> check = checkBroadcast(save[buffer[2]], save[buffer[4]]);
        if (check.first) {
            Tensor* binary =
            new TensorBinaryOperation(buffer[2], buffer[4], buffer[3], check.second, save);
            save[buffer[0]] = binary;
        } else {
            std::cout << "Unable to broadcast. Node defining failed" << std::endl;
        }

    } else {
        std::cout << "invalid input type" << std::endl;
    }
}



void Inputevalnum(int answer_num) {
    std::string str;
    std::string tmp;
    std::vector<std::string> buffer;
    getline(std::cin, str);
    std::stringstream ss(str);
    while (ss >> tmp) {
        buffer.push_back(tmp);
    }
    if (buffer[0] == "EVAL") {
        std::string target = buffer[1];
        if (buffer.size() == 2) {  //如果直接输出某结点而不赋值
            if (save[target]->calculate()) {
                save[target]->display();
            }
            save[target]->initialize();
            return;
        }
        
        if (save[target]->calculate()) {
            save[target]->display();
        }
        
    //     int num = stoi(buffer[2]);
    //     for (int i = 0; i < num; i++) {
    //         std::vector<double> tmp;
    //         tmp.push_back(stod(buffer[2 * i + 4]));
    //         // save[buffer[2 * i + 3]]->set(stod(buffer[2 * i + 4]));
    //     }
    //     if (save[target]->calculate()) {
    //         std::cout << std::fixed << std::setprecision(4)
    //         << save[target]->value() << std::endl;
    //         answer[answer_num] = save[target]->value();
    //     }
    //     save[target]->reiscal();  // 刷新结点的状态
    //     return;
    // } else if (buffer[0] == "SETCONSTANT") {
    //     double tmp_var = stod(buffer[2]);
    //     save[buffer[1]]->set(tmp_var);
    // } else if (buffer[0] == "SETANSWER") {
    //     int tmp_var = stoi(buffer[2]);
    //     save[buffer[1]]->set(answer[tmp_var]);
    // }
}
