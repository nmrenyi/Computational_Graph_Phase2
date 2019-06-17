#include"_Tensor.h"
#include<iostream>
// Tensor类成员函数的实现
Tensor::Tensor() {}
void Tensor::initialize() {
    calculated = 0;  // 使节点处于未调用
    for (auto x : input) {
        x -> initialize();
    }
}
std::vector<int> Tensor:: getDim() const {
    return dim;
}
void Tensor::display() const {
    #ifdef debug
    std::cout << "in display, data.size() = " << data.size()  << std::endl;
    #endif
    for (auto x : data) {
        std::cout << x << " ";
    }
}
void Tensor::setData(std::vector<double> data_) {
    data = data_;
    calculated = true;
}
std::vector<double> Tensor::getData() const {
    return data;
}


Tensor::~Tensor() {}


// TensorConstant类成员函数的实现
TensorConstant:: TensorConstant(std::vector<int>dimInfo,
    std::vector<double>dataInfo) {
    dim = dimInfo;
    data = dataInfo;
    #ifdef debug
        std::cout << "Constant built" << std::endl;
    #endif
}

// std::vector<double> TensorConstant::getData() {
//     return data;
// }

// std::vector<int> TensorConstant:: getDim() {
//     #ifdef debug
//         std::cout << "getting dim = " << dim.size() << std::endl;
//     #endif
//     return dim;
// }

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
TensorVariable::TensorVariable(std::vector<int>dimInfo,
    std::vector<double>dataInfo) {
    dim = dimInfo;
    data = dataInfo;
}
bool TensorVariable:: calculate() {
    return true;  // Variable也一定是有值的
}


// TensorSingleOperation类成员函数的实现
TensorSingleOperation::TensorSingleOperation(std::string target,
    std::string operatorName, std::map<std::string, Tensor*>& save) {
    operationName = operatorName;
    input.push_back(save[target]);
}
bool TensorSingleOperation::calculate() {
    return true;
}

// TensorBinaryOperation类成员函数的实现
TensorBinaryOperation::TensorBinaryOperation(std::string target1,
    std::string target2, std::string operatorName, std::vector<int>dimInfo,
    std::map<std::string, Tensor*>& save) {
    operationName = operatorName;
    dim = dimInfo;
    input.push_back(save[target1]);
    input.push_back(save[target2]);
}

std::vector<double> broadcast(std::vector<int>v,
    std::vector<int>standard, std::vector<double>data);
// 仅仅支持二维矩阵
bool TensorBinaryOperation::calculate() {
    // 判断此节点是否被调用过,保证每个节点值
    if (calculated) {
        return true;
    }
    bool flag = (input[0]->calculate())
        && (input[1]->calculate());
    if (flag) {
        std::vector<int>v1 = input[0]->getDim();
        std::vector<int>v2 = input[1]->getDim();
        std::vector<int>standard = dim;

        // 按照standard所示维数，对v1对应的data进行broadcast
        std::vector<double>info1 = broadcast(v1, standard, input[0]->getData());
        std::vector<double>info2 = broadcast(v2, standard, input[1]->getData());
        int dataSize = info1.size();  // 获取data的数量
        data.clear();
        if (operationName == "+") {
            for (int i = 0; i < dataSize; i++) {
                data.push_back(info1[i] + info2[i]);
            }
        } else if (operationName == "*") {
            for (int i = 0; i < dataSize; i++) {
                data.push_back(info1[i] * info2[i]);
            }
        } else if (operationName == "-") {
            for (int i = 0; i < dataSize; i++) {
                data.push_back(info1[i] - info2[i]);
            }
        } else {
            std::cout << "invalid operator name" << std::endl;
        }
    } else {
        return false;
    }
    return true;
}
// 只是一个声明，方便调用另外一个cpp当中的函数
int getDataNum(std::vector<int>dim);

// 根据v的情况，把data broadcast到standard这个大小
std::vector<double> broadcast(std::vector<int>v,
    std::vector<int>standard, std::vector<double>data) {
    if (v == standard) {
        return data;
    }
    std::vector<double> r = data;
    if (v.size() == standard.size()) {
        // 未能实现任一维数的broadcast
        // for (int i = standard.size() - 1; i >= 0; i++) {
        //     if (v[i] != standard[i]) {
        //         int minus = standard[i] - v[i];
        //         std::vector<int>tmp(standard.begin(),
                // standard.begin() + i - 1);
        //         int num = getDataNum(tmp);
        //         for (int j = num; j > 0; j--) {
        //             int target = *(r.begin() + j - 1);
        //             std::vector<int>includes()
        //             std::vector<double>another(r.begin(),
                        // r.begin() + v[i], );
        //         }
        //     }
        // }
        if (v[1] != standard[1]) {  // 第二维大小不同
            int minus = standard[1]- v[1];  // 要复制多少个这个元素
            for (int i = v[0]; i > 0; i--) {  // 有这么多元素需要复制
                int target = *(r.begin() + (i - 1) * v[1]);  // 当前要复制的元素
                // 缺少的元素重复minus次
                std::vector<double>another(minus, target);
                r.insert(r.begin() + (i - 1) * v[1] + 1,
                    another.begin(), another.end());
            }
        }
        if (v[0] != standard[0]) {
            std::vector<double>another(r.begin(),
                r.begin() + standard[1]);  // 需要复制的内容
            int minus = standard[0] - v[0];  // 复制次数
            for (int i = 0; i < minus; i++) {
                r.insert(r.end(), another.begin(), another.end());
            }
        }
    } else {  // 维数不相等的时候，把它补成相等的
        int minus = standard.size() - v.size();
        std::vector<int>another(minus, 1);
        v.insert(v.begin(), another.begin(), another.end());
        r = broadcast(v, standard, data);
    }
    return r;
}

// TensorReshapeOperation类成员函数的实现
TensorReshapeOperation::TensorReshapeOperation(std::string target,
    std::vector<int> aimDim, 
    std::map<std::string, Tensor*>& save) {
    input.push_back(save[target]);
    // data = save[target]->getData();
    dim = aimDim;
}

bool TensorReshapeOperation::calculate() {
    if (calculated) {
        return true;
    }
    bool flag = input[0]->calculate();
    if (flag) {
        if (getDataNum(dim) == getDataNum(input[0]->getDim())) {
            data = input[0]->getData();
            calculated = true;
            return true;
        } else {
            std::cout << 
            "Unable to reshape. The number of data is wrong."
            << std::endl;
            return false;
        }
    } else {
        return false;
    }
}

TensorConcatOperation::TensorConcatOperation(std::string target1, 
    std::string target2, int concatway, std::map<std::string, Tensor*>& save){
    concatWay = concatway;
    input.push_back(save[target1]);
    input.push_back(save[target2]);
}

bool TensorConcatOperation::calculate() {
    if (calculated) {
        return true;
    }
    bool flag1 = input[0]->calculate();
    bool flag2 = input[1]->calculate();
    if (flag1 && flag2) {
        std::vector<int> dim1 = input[0]->getDim();
        std::vector<int> dim2 = input[1]->getDim();
        std::vector<double> data1 = input[0]->getData();
        std::vector<double> data2 = input[1]->getData();
        int size1 = dim1.size();
        int size2 = dim2.size();
        if (size1 != size2) { // 两个tensor维数不一样，无法concat
            return false;
        }
        // 判断能否concat的条件：只有拼在一起的那个轴长度可以不一样，剩下都得一样
        bool canConcat = 1;
        for (int i = 0; i < size1 ; i++) {
            if (i != concatWay && dim1[i] != dim2[i]) {
                canConcat = 0;
                break;
            }
        }
        if (!canConcat) {
            return false;
        }

        // 比较简单地，先给出结点concat的dim
        dim = dim1;
        dim[concatWay] = dim1[concatWay] + dim2[concatWay];

        //然后给出data
        int groupNum1 = 1;//每次向thisdata中加入一个组团的data数量
        int groupNum2 = 1;
        for (int i = size1-1; i != concatWay - 1; i--) {//计算两个groupNum
            groupNum1 *= dim1[i]; 
            groupNum2 *= dim2[i];
        }
        int totalDataNum = getDataNum(dim);//此concat后的结点的dataNum
        
        int placedNum = 0;// 已经向新data(thisdata)中放入的数字的数量
        std::vector<double> thisdata;
        int N = 0; // 第N个回合，每个回合放入data1与data2中各一个组团
        while (placedNum != totalDataNum) {
            for (int i = 0; i < groupNum1; i++) {
                thisdata.push_back(data1[N*groupNum1+i]);
            }
            for (int i = 0; i < groupNum2; i++) {
                thisdata.push_back(data2[N*groupNum2+i]);
            }
            placedNum += groupNum1;
            placedNum += groupNum2;
            N++;
        }
        setData(thisdata);
        calculated = true;
        return true;
    } else {
        return false;
    }
}