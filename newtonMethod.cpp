#include "Node.h"
#include<string>
#include<vector>
#include<map>
#include<iomanip>
#include<iostream>
class Counter {
 private:
    int count;
    Counter() { count = 0; }
    Counter(const Counter&) = delete;
    void operator= (const Counter&) = delete;
    ~Counter() {}
 public:
    static Counter& instance() {
        static Counter _instance;
        return _instance;
    }
    void addCount() { count++; }
    int getCount() { return count; }
};
// int k = 0;  // 命名计数器

base* makePower(int degree, std::map <std::string, base*>& store, Counter& c) {
    if (degree == 1) {
        return store["x"];
    }
    c.addCount();
    std::string next_name = "Multi" + std::to_string(c.getCount());
    store[next_name] = makePower(degree - 1, store, c);
    return (new binaryoperation("x", next_name, "*", store));
}

base* makeMulti(double coefficient, int degree, std::map <std::string, base*>& store, Counter& c) {

    if (degree == 0) {
        base* tmp = new Constant;
        tmp->set(coefficient);
        return tmp;
    }
    std::string constant = std::to_string(coefficient);
    store[constant] = new Constant;
    store[constant]->set(coefficient);
    c.addCount();
    std::string multi_name = "Multi" + std::to_string(c.getCount());
    store[multi_name] = makePower(degree, store, c);
    return (new binaryoperation(constant, multi_name, "*", store));
}
base* makePlus(std::vector<double> equation,
std::map <std::string, base*>& store, Counter& c) {
    if (equation.size() == 3) {  // 需要处理的vector是剩下一个系数没有处理了(剩下两个是初始值和最高次数)
        return makeMulti(equation[0], equation[2], store, c);
    }
    c.addCount();
    std::string prev_name = "Node" + std::to_string(c.getCount());
    c.addCount();
    std::string next_name = "Node" + std::to_string(c.getCount());
    store[prev_name] =
    makeMulti(equation[0], equation.back() - (equation.size() - 3), store, c);
    equation.erase(equation.begin());
    store[next_name] = makePlus(equation, store, c);
    return (new binaryoperation(prev_name, next_name, "+", store));
}

void iteration(double initial_value, std::string target,
std::map <std::string, base*> store, int m) {
    if (m == 5) {  // five iteration completed
        return;
    }
    store["x"]->set(initial_value);
    store[target]->calculate();
    double func_value = store[target]->value();
    store[target]->derivate(1.0);
    double func_deri_value = store["x"]->get_deri();
    if (func_deri_value == 0) {
        std::cout << "Derivation equals to 0, iteration terminated."
        << std::endl;
        return;
    }
    double new_value = -(func_value / func_deri_value) + store["x"]->value();
    std::cout << std::fixed << std::setprecision(4) << new_value << " ";
    store[target]->reiscal();
    iteration(new_value, "result", store, m + 1);
}

void newtonMethod(std::vector<double> equation) {
    Counter& c = Counter::instance();
    std::map <std::string, base*> store;
    store["x"] = new Placeholder;
    store["result"] = makePlus(equation, store, c);
    double initial_value = equation[equation.size() - 2];
    iteration(initial_value, "result", store, 0);
    std::cout << std::endl;
}
