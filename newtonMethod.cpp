#include "Node.h"
#include<string>
#include<vector>
#include<map>
#include<iomanip>
#include<iostream>
int k = 0;  // 命名计数器

base* makePower(int degree, std::map <std::string, base*>& store) {
    if (degree == 1) {
        return store["x"];
    }
    std::string next_name = "Multi" + std::to_string(k++);
    store[next_name] = makePower(degree - 1, store);
    return (new binaryoperation("x", next_name, "*", store));
}

base* makeMulti(double coefficient, int degree, std::map <std::string, base*>& store) {

    if (degree == 0) {
        base* tmp = new Constant;
        tmp->set(coefficient);
        return tmp;
    }
    std::string constant = std::to_string(coefficient);
    store[constant] = new Constant;
    store[constant]->set(coefficient);
    std::string multi_name = "Multi" + std::to_string(k++);
    store[multi_name] = makePower(degree, store);
    return (new binaryoperation(constant, multi_name, "*", store));
}
base* makePlus(std::vector<double> equation, std::map <std::string, base*>& store) {
    if (equation.size() == 3) {
        return makeMulti(equation[0], equation[2], store);
    }
    std::string prev_name = "Node" + std::to_string(k++);
    std::string next_name = "Node" + std::to_string(k++);
    store[prev_name] = makeMulti(equation[0], equation.back() - (equation.size() - 3), store);
    equation.erase(equation.begin());
    store[next_name] = makePlus(equation, store);
    return (new binaryoperation(prev_name, next_name, "+", store));
}

void newtonMethod(std::vector<double> equation) {
    std::map <std::string, base*> store;
    store["x"] = new Placeholder; 
    store["result"] = makePlus(equation, store);
    double initial_value = equation[equation.size() - 2];
    store["x"]->set(initial_value);
    store["result"]->calculate();
    std::cout << std::fixed << std::setprecision(4)
            << store["result"]->value() << std::endl;
    // iteration(equation.end() - 2, "result", store, 0);
}
