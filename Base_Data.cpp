#include"_Base_Data.h"
#include"_My_Define.h"

set<int> Base_Data::my_error_set {0}; // 类外定义静态变量
Base_Data::Base_Data() {}
Base_Data::Base_Data(float value):data(value) {}
Base_Data::~Base_Data() {
    delete ptr[0]; ptr[0] = nullptr;
    delete ptr[1]; ptr[1] = nullptr;
}

float Base_Data::set_data(float dst) {//改变data的接口
    caled = 1;
    return data = dst;
}

float Base_Data::get_data() {//获取data的接口
    return data;
}

void Base_Data::set_ptr(Base_Data* left,Base_Data* right) {
    ptr[0] = left;
    ptr[1] = right;
}

//基类的虚函数类外定义，没有实际用处，直接返回(0, 0.0)
std::pair<bool, float> Base_Data::calc() {
    return std::make_pair(0, 0.0);
}

void Base_Data::init_caled() { //记录是否计算过的变量caled初始化为0
    caled = 0;
}

void Base_Data::init_set() { //初始化存储error的set
    my_error_set.clear();
}