#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<algorithm>
#include"_Tensor.h"

int getDataNum(std::vector<int>dim) {
    int tmp = 1;
    for (auto x : dim) {
        tmp *= x;
    }
    return tmp;
}
void inputNode(std::map<std::string, Tensor*>& save) {
    std::string name;
    char type;
    int dimNum = 0;
    std::vector<int>dim;
    // x P 2  分别是节点名字，属性，共有的维数
    std::cin >> name >> type >> dimNum;
    for (int i = 0; i < dimNum; i++) {
        int tmp = 0;
        std::cin >> tmp;  // 2 1  分别是每一维的长度
        dim.push_back(tmp);
    }

    switch (type) {  // 判断输入的变量的类型
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
            #ifdef debug
                std::cout << "making constant complete" << std::endl;
            #endif
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
    std::vector<int> tmpDim1 = ptr1->getDim();
    std::vector<int> tmpDim2 = ptr2->getDim();
    std::vector<int>tmp;  // 储存加节点的维数信息
    std::vector<int> dim1 = (tmpDim1.size() >= tmpDim2.size())
        ? tmpDim1 : tmpDim2;
    std::vector<int> dim2 = (tmpDim1.size() < tmpDim2.size())
        ? tmpDim1 : tmpDim2;
    int minus = dim1.size() - dim2.size();
    std::vector<int> another(minus, 1);
    dim2.insert(dim2.begin(), another.begin(), another.end());

    for (unsigned int i = 1; i <= dim1.size(); i++) {
        int length1 = *(dim1.end() - i);
        int length2 = *(dim2.end() - i);
        // 倒序将合成节点的维数加入vector
        tmp.push_back(std::max(length1, length2));
        if (length1 != length2 && length1 != 1 && length2 != 1) {
            return std::make_pair(false, tmp);
        }
    }
    std::reverse(tmp.begin(), tmp.end());  // 将倒序的vector反过来
    return std::make_pair(true, tmp);
}

void inputOperate(std::map<std::string, Tensor*>& save) {
    std::string str;
    std::string tmp;
    std::vector <std::string> buffer;  // 用sstream存储当前输入的关键信息
    getline(std::cin, str);

    std::stringstream ss(str);
    while (ss >> tmp) { buffer.push_back(tmp); }

    if (buffer.size() == 4) {  // a = SIN x 单目运算符
        Tensor* single = new TensorSingleOperation(buffer[3], buffer[2], save);
        save[buffer[0]] = single;
    } else if (buffer.size() == 5 && buffer[2] != "RESHAPE") {
        // a = x + y 双目运算符
        auto check = checkBroadcast(save[buffer[2]], save[buffer[4]]);
        if (check.first) {
            Tensor* binary =
            new TensorBinaryOperation(buffer[2], buffer[4],
            buffer[3], check.second, save);
            save[buffer[0]] = binary;
        } else {
            std::cout <<
            "Unable to broadcast. Node \"" << buffer[0] <<"\" defining failed"
            << std::endl;
        }

    } else if (buffer.size() == 5 && buffer[2] == "RESHAPE") {
        // a = RESHAPE b 3
        int aimDimNum = std::stoi(buffer[4]);
        std::vector<int> aimDim;
        int tmp;
        for (int i = 0; i < aimDimNum; i++) {
            std::cin >> tmp;
            aimDim.push_back(tmp);
        } 
        Tensor* reshape = new TensorReshapeOperation(buffer[3], aimDim, save);
        save[buffer[0]] = reshape;
        getchar();
    } else if (buffer.size() == 6) {  // a = CONCAT b c 2
        int concatway = std::stoi(buffer[5]);
        Tensor* concat =
        new TensorConcatOperation(buffer[3], buffer[4], concatway, save);
        save[buffer[0]] = concat;
    } else {
        std::cout << "invalid input type" << std::endl;
    }
}

void changePara(std::map<std::string, Tensor*>& save, std::string info) {
    std::string tmp;
    std::stringstream ss(info);
    std::vector<std::string>buffer;
    while (ss >> tmp) {
        buffer.push_back(tmp);
    }
    unsigned int dimNum = getDataNum(save[buffer[0]]->getDim());
    if (dimNum == buffer.size() - 2) {
        std::vector<double> data;
        for (unsigned int i = 1; i < buffer.size() - 1; i++) {
            data.push_back(stod(buffer[i]));
        }
        save[buffer[0]]->setData(data);
    } else {
        std::cout << "invalid parameter number" << std::endl;
    }
}

void inputEvalNum(int answer_num, std::map<std::string, Tensor*>& save) {
    std::string str;
    std::string tmp;
    std::vector<std::string> buffer;  // buffer为储存赋值运算的vector
    getline(std::cin, str, '#');  // EVAL a 1 x 1 2 # y 2 1 #
    std::stringstream ss(str);
    while (ss >> tmp) {
        buffer.push_back(tmp);
    }
    buffer.push_back("#");
    if (buffer[0] == "EVAL") {
        std::string target = buffer[1];  // 要计算的对象
        if (buffer.size() == 3) {  // 不赋值，直接输出 EVAL x #
            if (save[target]->calculate()) {
                save[target]->display(save[target]->getDim(), 0);
            }
        } else {  // 有赋值的过程
            int paraNum = stoi(buffer[2]);  // 此次EVAL所需参数个数
            std::string subString;
            for (unsigned int i = 3; i < buffer.size(); i++) {
                subString += buffer[i];
                subString += " ";
            }
            changePara(save, subString);
            for (int i = 0; i < paraNum - 1; i++) {
                std::string strInfo;
                getline(std::cin, strInfo, '#');
                strInfo += " #";
                changePara(save, strInfo);
            }
            if (save[target]->calculate()) {
                save[target]->display(save[target]->getDim(), 0);
            }
        }
        save[target]->initialize();
    } else {
        std::cout << "invalid input command" << std::endl;
    }
}
