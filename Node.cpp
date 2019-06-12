#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include "_Node.h"

void base::reiscal() {
    deri_value = 0;
    iscal = false;
    // 递归调用清空所有被调用节点的计算状态
    // 保证每次执行新的EVAL时结点状态都是未调用
    for (auto p : input) {
        p->reiscal();
        p->reset();
    }
}

bool base::set_visit_num() {
    visit_num++;
    if (visit_num == pres)
        return true;
    return false;
}

bool Placeholder::calculate() {
    pres++;
    // 判断该变量是否被赋过值
    if (iscalculated()) {
        return true;
    } else {
        std::cout << "ERROR: Placeholder missing" << std::endl;
        return false;
    }
}


bool singleoperation::calculate() {
    pres++;  // 前驱节点数目加一
    // 判断此节点是否被调用过，保证一个节点只计算一次
    if (iscalculated())
        return true;
    if (!input[0]->calculate()) {  // 如果缺乏赋值不能计算
        return false;
    } else {  // 判断计算类型
        if (operationname == "SIN") {
            set(sin(input[0]->value()));
        } else if (operationname == "LOG") {
            // 判断定义域
            if (input[0]->value() <= 0) {
                std::cout
                << "ERROR: LOG operator's input must be positive"
                << std::endl;
                return false;
            } else {
                set(log(input[0]->value()));
            }
        } else if (operationname == "EXP") {
            set(exp(input[0]->value()));
        } else if (operationname == "TANH") {
            set(tanh(input[0]->value()));
        } else if (operationname == "SIGMOID") {
            set((1 / (1 + exp(-(input[0]->value())))));
        } else if (operationname == "PRINT") {
            set(input[0]->value());
            std::cout << "PRINT operator: " << nodename << " = ";
            std::cout << std::fixed << std::setprecision(4)
            << input[0]->value() << std::endl;
        } else if (operationname == "ASSERT") {
            if (input[0]->value() > 0) {
                set(0.0);
                return true;
            } else {
                std::cout
                << "ERROR: Assertion failed"
                << std::endl;
                return false;
            }
        }
        return true;
    }
}

bool binaryoperation::calculate() {
    pres++;  // 前驱节点数目加一
    // 判断此节点是否被调用过，保证一个节点只计算一次
    if (iscalculated())
        return true;

    // 判断双目运算符的两个参数是否能够计算
    bool flag = (input[0]->calculate()) && (input[1]->calculate());
    if (flag) {
        if (operationname == "+") {
            double tmp = input[0]->value() + input[1]->value();
            set(tmp);
        } else if (operationname == "-") {
            double tmp = input[0]->value() - input[1]->value();
            set(tmp);
        } else if (operationname == "*") {
            double tmp = input[0]->value() * input[1]->value();
            set(tmp);
        } else if (operationname == "/") {
            if (input[1]->value() != 0) {
                double tmp = input[0]->value() / input[1]->value();
                set(tmp);
            } else {
                std::cout << "ERROR: Division by zero" << std::endl;
                return false;
            }
        } else if (operationname == ">") {
            if (input[0]->value() > input[1]->value())
                set(1.0);
            else
                set(0.0);
        } else if (operationname == ">=") {
            if (input[0]->value() >= input[1]->value())
                set(1.0);
            else
                set(0.0);
        } else if (operationname == "<") {
            if (input[0]->value() < input[1]->value())
                set(1.0);
            else
                set(0.0);
        } else if (operationname == "<=") {
            if (input[0]->value() <= input[1]->value())
                set(1.0);
            else
                set(0.0);
        } else if (operationname == "==") {
            if (input[0]->value() == input[1]->value())
                set(1.0);
            else
                set(0.0);
        }
        return true;
    } else {
        return false;
    }
}

bool bindoperation::calculate() {
    if (iscalculated())
        return true;
    bool flag = (input[0]->calculate()) && (input[1]->calculate());
    if (flag) {
        double tmp = input[0]->value();
        set(tmp);
        return true;
    } else {
        return false;
    }
}

bool AssignOperation::calculate() {
    #ifdef DEBUG
    std::cout << "hello in assign::cal" << std::endl;
    #endif
    if (iscalculated())
        return true;
    bool flag = input[1]->calculate();
    if (flag) {
        double tmp = input[1]->value();
        set(tmp);
        input[0]->set(tmp);
        return true;
    } else {
        return false;
    }
}

bool COND::calculate() {
    pres++;  // 前驱节点数目加一
    // 判断此节点是否被调用过，保证一个节点只计算一次
    if (iscalculated())
        return true;
    // 首先判断第一个是否能够计算（或者是否已经存在值）
    if (input[0]->calculate()) {
        if (input[0]->value() > 0) {
            // 大于零只需判断第一个是否有值，故这样写
            if (input[1]->calculate()) {
                set(input[1]->value());
                return true;
            }
            return false;
        } else {
            if (input[2]->calculate()) {
                set(input[2]->value());
                return true;
            }
            return false;
        }
    }
    return false;
}

bool GRAD::calculate() {
    if (iscalculated())
        return true;
    if (input[0]->calculate()) {
        this->derivate(1.0);
        iscal = true;
        return true;
    }
    return false;
}

bool AT::calculate() {
    if (iscalculated())
        return true;
    if (input[0]->calculate()) {
        set(input[1]->get_deri());
        return true;
    }
    return false;
}

void Placeholder::derivate(double deri_value) {
    set_deri(deri_value);
}

void singleoperation::derivate(double deri_value) {
    set_deri(deri_value);
    if (set_visit_num()) {
        if (operationname == "SIN") {
            input[0]->derivate(get_deri() * cos(input[0]->value()));
        } else if (operationname == "LOG") {
            input[0]->derivate(get_deri() / input[0]->value());
        } else if (operationname == "EXP") {
            input[0]->derivate(get_deri() * exp(input[0]->value()));
        } else if (operationname == "TANH") {
            input[0]->derivate(get_deri() *
                (1 - input[0]->value() * input[0]->value()));
        } else if (operationname == "SIGMOID") {
            input[0]->derivate(get_deri() *
                input[0]->value() * (1 - input[0]->value()));
        } else if (operationname == "PRINT") {
            input[0]->derivate(get_deri());
        } else if (operationname == "ASSERT") {
            if (input[0]->value() > 0) {
                input[0]->derivate(get_deri() * 1.0);
            } else {
                std::cout
                << "ERROR: Assertion failed"<< std::endl;
            }
        }
    }
}

void binaryoperation::derivate(double deri_value) {
    set_deri(deri_value);
    if (set_visit_num()) {
        // debug
        if (operationname == "+") {
            input[0]->derivate(get_deri() * 1);
            input[1]->derivate(get_deri() * 1);
        } else if (operationname == "-") {
            input[0]->derivate(get_deri() * 1);
            input[1]->derivate(get_deri() * -1);
        } else if (operationname == "*") {
            input[0]->derivate(get_deri() * input[1]->value());
            input[1]->derivate(get_deri() * input[0]->value());
        } else if (operationname == "/") {
            input[0]->derivate(get_deri() / input[1]->value());
            input[1]->derivate(get_deri() *
                (- input[0]->value() / input[1]->value() / input[1]->value()));
        } else if (operationname == ">") {
            input[0]->derivate(get_deri() > 0 ? get_deri() : -get_deri());
            input[1]->derivate(get_deri() > 0 ? -get_deri() : get_deri());
        } else if (operationname == ">=") {
            input[0]->derivate(get_deri() >= 0 ? get_deri() : -get_deri());
            input[1]->derivate(get_deri() >= 0 ? -get_deri() : get_deri());
        } else if (operationname == "<") {
            input[0]->derivate(get_deri() < 0 ? get_deri() : -get_deri());
            input[1]->derivate(get_deri() < 0 ? -get_deri() : get_deri());
        } else if (operationname == "<=") {
            input[0]->derivate(get_deri() <= 0 ? get_deri() : -get_deri());
            input[1]->derivate(get_deri() <= 0 ? -get_deri() : get_deri());
        } else if (operationname == "==") {
            input[0]->derivate(get_deri() == 0 ? get_deri() : -get_deri());
            input[1]->derivate(get_deri() == 0 ? -get_deri() : get_deri());
        }
    }
}

void bindoperation::derivate(double deri_value) {
    set_deri(deri_value);
    input[0]->derivate(get_deri());
    input[1]->derivate(1.0);
}

void COND::derivate(double deri_value) {
    set_deri(deri_value);
    input[0]->derivate(0);
    input[1]->derivate(input[0]->value() > 0 ? get_deri() : -get_deri());
    input[2]->derivate(input[0]->value() > 0 ? -get_deri() : get_deri());
}

void GRAD::derivate(double deri_value) {
    set_deri(deri_value);
    input[0]->derivate(get_deri());
}

