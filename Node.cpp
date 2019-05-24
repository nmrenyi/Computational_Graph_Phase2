#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include "Node.h"

void base::reiscal() {
    iscal = false;
    // 递归调用清空所有被调用节点的计算状态
    // 保证每次执行新的EVAL时
    // 结点状态都是未调用
    for (int i = 0; i < input.size(); i++) {
        input[i]->reiscal();
    }
}

bool Placeholder::calculate() {
    // 判断该变量是否被赋过值
    if (iscalculated()) {
        return true;
    } else {
        std::cout << "ERROR: Placeholder missing" << std::endl;
        return false;
    }
}


bool singleoperation::calculate() {
    // 判断此节点是否被调用过，保证一个节点只计算一次
    if (iscalculated())
        return true;
    if (!input[0]->calculate()) {  // 如果缺乏赋值不能计算
        return false;
    } else {  // 判断计算类型
        if (operationname == "SIN") {
            set(sin(input[0]->value()));
        }
        if (operationname == "LOG") {
            // 判断定义域
            if (input[0]->value() <= 0) {
                std::cout
                << "ERROR: LOG operator's input must be positive"
                << std::endl;
                return false;
            } else {
                set(log(input[0]->value()));
            }
        }

        if (operationname == "EXP") {
            set(exp(input[0]->value()));
        }

        if (operationname == "TANH") {
            set(tanh(input[0]->value()));
        }
        if (operationname == "SIGMOID") {
            set((1 / (1 + exp(-(input[0]->value())))));
        }
        if (operationname == "PRINT") {
            set(input[0]->value());
            std::cout << "PRINT operator: " << nodename << " = ";
            std::cout << std::fixed << std::setprecision(4)
            << input[0]->value() << std::endl;
        }
        if (operationname == "ASSERT") {
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
    //判断此节点是否被调用过，保证一个节点只计算一次
    if (iscalculated())
        return true;

    //判断双目运算符的两个参数是否能够计算
    bool flag = (input[0]->calculate()) && (input[1]->calculate());
    if (flag) {
        if (operationname == "+") {
            double tmp = input[0]->value() + input[1]->value();
            set(tmp);
        }

        if (operationname == "-") {
            double tmp = input[0]->value() - input[1]->value();
            set(tmp);
        }
        if (operationname == "*") {
            double tmp = input[0]->value() * input[1]->value();
            set(tmp);
        }

        if (operationname == "/") {
            if (input[1]->value() != 0) {
                double tmp = input[0]->value() / input[1]->value();
                set(tmp);
            } else {
                std::cout << "ERROR: Division by zero" << std::endl;
                return false;
            }
        }

        if (operationname == ">") {
            if (input[0]->value() > input[1]->value())
                set(1.0);
            else
                set(0.0);
        }

        if (operationname == ">=") {
            if (input[0]->value() >= input[1]->value())
                set(1.0);
            else
                set(0.0);
        }

        if (operationname == "<") {
            if (input[0]->value() < input[1]->value())
                set(1.0);
            else
                set(0.0);
        }

        if (operationname == "<=") {
            if (input[0]->value() <= input[1]->value())
                set(1.0);
            else
                set(0.0);
        }

        if (operationname == "==") {
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

bool bindoperation::calculate(){
    if (iscalculated())
        return true;
    bool flag = (input[0]->calculate()) && (input[1]->calculate());
    if (flag) {
        double tmp = input[0]->value();
        set(tmp);
    } else {
        return false;
    }
}

bool COND::calculate() {
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
