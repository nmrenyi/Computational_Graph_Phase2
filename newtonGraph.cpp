#include "Node.h"
#include "Input.h"
#include<string>
#include<vector>
#include<map>

// 构建name节点
std::string makePlus(std::vector<double> equation, std::map <std::string, base*>& store, int k, std::string name, std::string prev_name) {
    if (equation.size() == 1) {  // 突突完了
        return ;  // ???
    }
    // 前序节点为乘节点
    store[std::to_string(equation[0])] = new Constant;  // 这块儿会不会冲突，我觉得不会
    store[std::to_string(equation[0])]->set(equation[0]);
    store[prev_name] = new binaryoperation(std::to_string(equation[0]), makeMulti(), "*", store);

    // 后面的节点为 + 节点
    std::string next_name = "plus" + std::to_string(k++);
    std::string ahead_name = "multi" + std::to_string(k++);
    // std::vector<double> tmp = equation;
    // tmp.erase(tmp.begin(), tmp.begin() + 2);
    equation.erase(equation.begin(), equation.begin() + 2);
    store[next_name] = new binaryoperation();
    store[name] = new binaryoperation(prev_name, makePlus(tmp, store, k, next_name), "+", store);
}

void newtonMethod(std::vector<double> equation) {
    std::map <std::string, base*> store;
    store["x"] = new Placeholder;

    // store["result"] = new binaryoperation("x0", makePlus(), store);

    // 第一步，分为 + 节点
    // 第二部，每个 + 节点分为 * 节点
}