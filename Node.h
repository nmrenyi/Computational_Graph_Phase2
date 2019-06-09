#pragma once
#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<cmath>

class base {
 protected:
    std::vector <base*> input;
    double output_value;
    bool iscal = false;
    double deri_value = 0; // 导数
    int pres = 0; // 前驱数目
    int visit_num = 0; // 访问次数
 public:
    // 给节点赋值，将当前节点修改为已调用
    void set(double x) {
        output_value = x;
        iscal = true;
    }
    double get_deri() { return deri_value; }
    void set_deri(double x) { deri_value += x; }
    bool set_visit_num();
    // 重置导数、前驱导数、前驱数目和访问次数
    void reset() { deri_value = pres = visit_num = 0; } 
    // 实现抽象类，返回值说明是否能够计算
    // (既返回值为false说明缺少计算所需的参数等而导致没有计算结果)
    virtual bool calculate() = 0;
    void reiscal();  // 刷新结点
    virtual double value() { return output_value; }  // 提供当前类的值
    virtual bool iscalculated() { return iscal; }  // 判断当前节点是否被调用过
    virtual void derivate(double pre_deri_value) {}; 
    virtual ~base() {}  // 虚析构函数
};

class Constant :public base {
 public:
    Constant() {}
    void set(double) = delete; // 确保常量不可以修改
    void derivate(double pre_deri_value) {}; 
    bool calculate() { return true; }  // 常量恒为真值
};

class Placeholder :public base {
 public:
    Placeholder() {}
    void derivate(double pre_deri_value); 
    bool calculate();
};

class Variable :public base {
 public:
    void derivate(double pre_deri_value) {}; 
    bool calculate() { return true; }  // V类也一定有值的存在
};

// 单目运算符
class singleoperation :public base {
    std::string operationname;
    std::string nodename;
 public:
    // 构造函数中通过赋值得到该节点的上一级结点的名称以及运算符并存储起来，以便后来的计算
    singleoperation(std::string a, std::string b,
        std::map <std::string, base*>& save) {
        input.push_back(save[a]);
        operationname = b;  // 得到该结点的操作名称
        nodename = a;
    }
    void derivate(double pre_deri_value);
    bool calculate();
};


class binaryoperation :public base {  // 双目运算符
    std::string operationname;
 public:
    // 构造函数中通过赋值得到该节点的上一级结点的名称以及运算符并存储起来，以便后来的计算
    binaryoperation(std::string a, std::string b, std::string c, 
        std::map <std::string, base*>& save) {
        input.push_back(save[a]);
        input.push_back(save[b]);
        operationname = c;  // 得到该结点的操作名称
    }
    void derivate(double pre_deri_value);
    bool calculate();
};

class bindoperation :public base {  // bind运算
 public:
    // 构造函数中通过赋值得到该节点的上一级结点的名称以及运算符并存储起来，以便后来的计算
    bindoperation(std::string a, std::string b,
        std::map <std::string, base*>& save) {
            input.push_back(save[a]);
            input.push_back(save[b]);
    }
    bool calculate();
    void derivate(double pre_deri_value);    
};

class AssignOperation :public base {  // assign操作
 public:
    AssignOperation(std::string a, std::string b,
        std::map <std::string, base*>& save) {
            input.push_back(save[a]);
            input.push_back(save[b]);
                #ifdef DEBUG
                std::cout << "hello in assign::assign" << std::endl;
                #endif
        }
    // void derivate(double pre_deri_value);
    bool calculate();
};


class COND :public base {
    std::string condname;
 public:
    //存储该节点的上一级结点
    COND(std::string a, std::string b, std::string c, std::string d, 
        std::map <std::string, base*>& save) {
        input.push_back(save[a]);
        input.push_back(save[b]);
        input.push_back(save[c]);
        condname = d;
    }
    void derivate(double deri_value);
    bool calculate();
};

class GRAD :public base {
 public:
    GRAD(std::string a, std::map <std::string, base*>& save) {
        input.push_back(save[a]);
    }
    void derivate(double pre_deri_value);
    bool calculate();
};

class AT :public base {
 public:
    AT(std::string a, std::string b,
        std::map <std::string, base*>& save) {
        input.push_back(save[a]);
        input.push_back(save[b]);
    }
    bool calculate();
}; 