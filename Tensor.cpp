#include"Tensor.h"
#include<iostream>
// Tensor类成员函数的实现
Tensor::Tensor() {}
void Tensor::initialize() {
    calculated = 0;  // 使节点处于未调用
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
TensorBinaryOperation::TensorBinaryOperation(std::string target1, std::string target2, std::string operatorName, std::map<std::string, Tensor*>& save) {
    operationName = operatorName;
    input.push_back(save[target1]);
    input.push_back(save[target2]);
}

bool TensorBinaryOperation::calculate() {
    // 判断此节点是否被调用过,保证每个节点值
    if (calculated) {
        return true;
    }


    return true;
}