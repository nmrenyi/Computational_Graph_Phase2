#include "_Input.h"
#include "_Node.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

// 存储结点名称到相应类对象的映射
std::map <std::string, Base*> save;
// 如果有输出值，存储当前对应操作数的输出结果
std::map <int, double> answer;
// 存储结点重名时将即将被覆盖的结点存储起来，便于释放内存
std::vector <Base*> save_for_delete;

void inputNode() {
    std::string name;
    char type;
    std::cin >> name;
    std::cin >> type;
    // 查找之前是否存在同名结点(指最开始的C、P、V类型)
    // 如果存在，将其push_back进save_for_delete以便后来删除而防止内存泄漏
    if (save.find(name) != save.end()) {
        save_for_delete.push_back(save[name]);
    }
    switch (type) {  // 判断输入的变量的类型
        case 'P': {
            Base* Newplace = new Placeholder;
            save[name] = Newplace;
        }
        break;
        case 'C': {
            double value;
            std::cin >> value;
            Base* Newconstant = new Constant(value);
            save[name] = Newconstant;
        }
        break;
        case 'V': {
            Base* Newvariable = new Variable;
            save[name] = Newvariable;
            double value;
            std::cin >> value;
            Newvariable->set(value);
        }
        break;
    }
}

void inputOperate() {
    std::string str;
    std::string tmp;
    std::vector <std::string> buffer;  // 用sstream存储当前输入的关键信息
    getline(std::cin, str);
    std::stringstream ss(str);
    while (ss >> tmp) { buffer.push_back(tmp); }

    // 查找之前是否存在同名结点（指单目、双目等运算符类型）
    // 如果存在，将其push_back进save_for_delete以便后来删除而防止内存泄漏
    if (save.find(buffer[0]) != save.end()) {
        save_for_delete.push_back(save[buffer[0]]);
    }

    if (buffer[2] == "GRAD") {  // GRAD
        Base* gradname = new GRAD(buffer[3], save);
        save[buffer[0]] = gradname;
    } else if (buffer[3] == "AT") {  // AT
        Base* atname = new AT(buffer[2], buffer[4], save);
        save[buffer[0]] = atname;
    } else if (buffer.size() == 4) {  // 单目运算符
        Base* singlename = new SingleOperation(buffer[3], buffer[2], save);
        save[buffer[0]] = singlename;
    } else if (buffer.size() == 5) {  // 双目运算符
        if (buffer[2] == "BIND") {
            Base* bindname =
            new BindOperation(buffer[3], buffer[4], save);
            save[buffer[0]] = bindname;
        } else if (buffer[2] == "ASSIGN") {  // ASSIGN 操作
            Base* assignName =
            new AssignOperation(buffer[3], buffer[4], save);
            save[buffer[0]] = assignName;
        } else {
            Base* binaryname =
            new BinaryOperation(buffer[2], buffer[4], buffer[3], save);
            save[buffer[0]] = binaryname;
        }
    } else if (buffer.size() == 6) {  // COND
        Base* condname =
        new COND(buffer[3], buffer[4], buffer[5], buffer[0], save);
        save[buffer[0]] = condname;
    }
}

void inputEvalNum(int answer_num) {
    std::string str;
    std::string tmp;
    std::vector<std::string> buffer;
    getline(std::cin, str);
    std::stringstream ss(str);
    while (ss >> tmp) {
        buffer.push_back(tmp);
    }
    if (buffer[0] == "EVAL") {
        std::string target = buffer[1];
        if (buffer.size() == 2) {  // 如果直接输出某结点而不赋值
            if (save[target]->calculate()) {
                std::cout << std::fixed << std::setprecision(4)
                << save[target]->value() << std::endl;
                answer[answer_num] = save[target]->value();
            }
            save[target]->reiscal();
            return;
        }
        int num = stoi(buffer[2]);
        for (int i = 0; i < num; i++) {
            save[buffer[2 * i + 3]]->set(stod(buffer[2 * i + 4]));
        }
        if (save[target]->calculate()) {
            std::cout << std::fixed << std::setprecision(4)
            << save[target]->value() << std::endl;
            answer[answer_num] = save[target]->value();
        }
        save[target]->reiscal();  // 刷新结点的状态
        return;
    } else if (buffer[0] == "SETCONSTANT") {
        double tmp_var = stod(buffer[2]);
        save[buffer[1]]->set(tmp_var);
    } else if (buffer[0] == "SETANSWER") {
        int tmp_var = stoi(buffer[2]);
        save[buffer[1]]->set(answer[tmp_var]);
    }
}


void delete_memory() {
    for (auto it = save.begin(); it != save.end(); it++) {
        delete it->second;
    }
    for (auto it = save_for_delete.begin(); it != save_for_delete.end(); it++) {
        delete (*it);
    }
}
