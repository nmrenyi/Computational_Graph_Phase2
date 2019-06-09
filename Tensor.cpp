#include"Tensor.h"
#include<iostream>
// Tensor类成员函数的实现
Tensor::Tensor() {}
void Tensor::initialize() {
    calculated = 0;  // 使节点处于未调用
}
std::vector<int> Tensor:: getDim() {
    return dim;
}
void Tensor::display() {}
Tensor::~Tensor(){}


// TensorConstant类成员函数的实现
TensorConstant:: TensorConstant(std::vector<int>dimInfo, std::vector<double>dataInfo) : dim(dimInfo), data(dataInfo) {}
bool TensorConstant:: calculate() {  // 常量始终可算
    return true;
}


// TensorPlaceholder类成员函数的实现
TensorPlaceholder::TensorPlaceholder(std::vector<int>dimInfo) {
    dim = dimInfo;
}
bool TensorPlaceholder:: calculate() {
    if (calculated) {
        return true;
    } else {
        std::cout << "ERROR: Placeholder missing" << std::endl;
        return false;
    }
}


// TensorVariable类成员函数的实现
TensorVariable::TensorVariable(std::vector<int>dimInfo, std::vector<double>dataInfo) {
    dim = dimInfo;
    data = dataInfo;
}
bool TensorVariable:: calculate() {
    return true;  // Variable也一定是有值的
}


// TensorSingleOperation类成员函数的实现
TensorSingleOperation::TensorSingleOperation(std::string target, std::string operatorName, std::map<std::string, Tensor*>& save) {
    operationName = operatorName;
    input.push_back(save[target]);
}
bool TensorSingleOperation::calculate() {
    return true;
}

// TensorBinaryOperation类成员函数的实现
TensorBinaryOperation::TensorBinaryOperation(std::string target1, std::string target2, std::string operatorName, std::vector<int>dimInfo, std::map<std::string, Tensor*>& save) {
    if (checkBroadcastable(save[target1], save[target2])) {
        operationName = operatorName;
        dim = dimInfo;
        input.push_back(save[target1]);
        input.push_back(save[target2]);
    } else {
        std::cout <<  "ValueError: frames are not aligned. Node defining failed." << std::endl;
    }
}


bool TensorBinaryOperation::calculate() {
    // 判断此节点是否被调用过,保证每个节点值
    if (calculated) {
        return true;
    }
    
    bool flag = (input[0]->calculate()) && (input[1]->calculate());
    if (flag) {
        if (operationName == "+") {
            std::vector<int>v1 = input[0]->getDim();
            std::vector<int>v2 = input[1]->getDim();
            if (v1 == v2) {
                for (int i = 0; i < v1.size(); i++) {
                    dim.push_back(v1[i] + v2[i]);
                }
            } else {
                
            }
        }
    } else {
        return false;
    }

    return true;
}