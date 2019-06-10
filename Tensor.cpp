#include"Tensor.h"
#include<iostream>
// Tensor类成员函数的实现
Tensor::Tensor() {}
void Tensor::initialize() {
    calculated = 0;  // 使节点处于未调用
    for (auto x : input) {
        x -> initialize();
    }
}
std::vector<int> Tensor:: getDim() {
    return dim;
}
void Tensor::display() {}
void Tensor::setData(std::vector<double> data_) {
    data = data_;
}
std::vector<double> Tensor::getData() {
    return data;
}


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
    operationName = operatorName;
    dim = dimInfo;
    input.push_back(save[target1]);
    input.push_back(save[target2]);
}


// 仅仅支持二维矩阵
bool TensorBinaryOperation::calculate() {
    // 判断此节点是否被调用过,保证每个节点值
    if (calculated) {
        return true;
    }
    
    bool flag = (input[0]->calculate()) && (input[1]->calculate());
    if (flag) {
        if (operationName == "+") {
            data.clear();
            std::vector<int>v1 = input[0]->getDim();
            std::vector<int>v2 = input[1]->getDim();
            std::vector<int>standard = dim;
            if (standard == v2 && standard == v1) {
                int dataSize = input[0]->getData().size();
                for (int i = 0; i < dataSize; i++) {
                    data.push_back(input[0]->getData()[i] + input[1]->getData()[i]);
                }
            } else {
                std::vector<double>info1 = input[0]->getData();
                std::vector<double>info2 = input[1]->getData();

                if (v1 != standard) {
                    info1 = broadcast(v1, standard, input[0]->getData());
                }
                if (v2 != standard) {
                    info2 = broadcast(v2, standard, input[1]->getData());
                }

                int dataSize = info1.size();
                for (int i = 0; i < dataSize; i++) {
                    data.push_back(info1[i] + info2[i]);
                }
            }
        }
    } else {
        return false;
    }

    return true;
}

// 根据v的情况，把data broadcast到standard这个大小
std::vector<double> broadcast(std::vector<int>v, std::vector<int>standard, std::vector<double>data) {
    std::vector<double> r = data;
    if (v.size() == standard.size()) {
        if (v[1] != standard[1]) {
            int minus = standard[1]- v[1];
            std::vector<double>another(minus, r[0]); // 这个缺少的元素，重复minus次
            
        }
    } else {

    }
}