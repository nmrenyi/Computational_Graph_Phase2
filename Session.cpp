#include <cstdlib>
#include <iostream>
#include "_Node.h"
#include "Session.h"

Session::Session() {}

Session::Session(std::stringstream& ss) {
    in.clear(); ss.clear(); 
    in.str(ss.str());
    in.seekg(ss.tellg()); 
    in.seekp(ss.tellp());
    Make_Graph();
}

Session::~Session() {
    Ses_delete_memory();
}

void Session::Make_Graph() {
    int Nodenum, operatenum;
    in >> Nodenum; 
    for (int i = 1; i <= Nodenum; i++) {  // 构建变量结点
        Ses_InputNode();
    }
    in >> operatenum;
    std::string str;
    getline(in, str); // 吸收换行符
    for (int i = 1; i <= operatenum; i++) {  // 构建运算符结点
        Ses_Inputoperate();
    }
}

void Session::Ses_InputNode() {
    std::string name;
    char type;
    in >> name;
    in >> type;
    // 查找之前是否存在同名结点(指最开始的C、P、V类型)
    // 如果存在，将其push_back进save_for_delete以便后来删除而防止内存泄漏
    if (save.find(name) != save.end()) {
        save_for_delete.push_back(save[name]);
    }
    switch (type) {  //判断输入的变量的类型
        case 'P': {
            base* Newplace = new Placeholder;
            save[name] = Newplace;
        }
        break;
        case 'C': {
            base* Newconstant = new Constant;
            save[name] = Newconstant;
            double value;
            in >> value;
            Newconstant->set(value);
        }
        break;
        case 'V': {
            base* Newvariable = new Variable;
            save[name] = Newvariable;
            double value;
            in >> value;
            Newvariable->set(value);
        }
        break;
    }
}

void Session::Ses_Inputoperate() {
    std::string str;
    std::string tmp;
    std::vector <std::string> buffer;  // 用sstream存储当前输入的关键信息
    getline(in, str);
    std::stringstream ss(str);
    while (ss >> tmp) { buffer.push_back(tmp); }

    // 查找之前是否存在同名结点（指单目、双目等运算符类型）
    // 如果存在，将其push_back进save_for_delete以便后来删除而防止内存泄漏
    if (save.find(buffer[0]) != save.end()) {
        save_for_delete.push_back(save[buffer[0]]);
    }

    if (buffer[2] == "GRAD") {  // GRAD
        base* gradname = new GRAD(buffer[3], save);
        save[buffer[0]] = gradname;
    } else if (buffer[3] == "AT") {  // AT
        base* atname = new AT(buffer[2], buffer[4], save);
        save[buffer[0]] = atname;
    } else if (buffer.size() == 4) {  // 单目运算符
        base* singlename = new singleoperation(buffer[3], buffer[2], save);
        save[buffer[0]] = singlename;
    } else if (buffer.size() == 5) {  // 双目运算符
        if (buffer[2] == "BIND") {
            base* bindname =
            new bindoperation(buffer[3], buffer[4], save);
            save[buffer[0]] = bindname;
        } else if (buffer[2] == "ASSIGN") {  // ASSIGN 操作
            base* assignName =
            new AssignOperation(buffer[3], buffer[4], save);
            save[buffer[0]] = assignName;
        } else {
            base* binaryname =
            new binaryoperation(buffer[2], buffer[4], buffer[3], save);
            save[buffer[0]] = binaryname;
        }
    } else if (buffer.size() == 6) {  // COND
        base* condname =
        new COND(buffer[3], buffer[4], buffer[5], buffer[0], save);
        save[buffer[0]] = condname;
    }
}

int Session::Ses_Inputevalnum(int answer_num) {
    std::string str;
    std::vector<std::string> buffer;
    std::cin >> str;
    buffer.push_back(str);
    if (buffer[0] == "EVAL") {
        std::string tmp;
        getline(std::cin, str);
        std::stringstream ss(str);
        while (ss >> tmp) {
            buffer.push_back(tmp);
        }
        std::string target = buffer[1];
        if (buffer.size() == 2) {  //如果直接输出某结点而不赋值
            if (save[target]->calculate()) {
                std::cout << std::fixed << std::setprecision(4)
                << save[target]->value() << std::endl;
                answer[answer_num] = save[target]->value();
            }
            save[target]->reiscal();
            return 1;
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
        return 1;
    } else if (buffer[0] == "SETCONSTANT") {
        double tmp_var = stod(buffer[2]);
        save[buffer[1]]->set(tmp_var);
    } else if (buffer[0] == "SETANSWER") {
        int tmp_var = stoi(buffer[2]);
        save[buffer[1]]->set(answer[tmp_var]);
    } else if (buffer[0] == "EOF" || buffer[0] == "eof") {
        return 0;
    } else if (buffer[0] == "SES") { // 代表新建SES或跳转SES
        return 2;
    } else if (buffer[0] == "RUN" || buffer[0] == "Run") {
        return 3;
    }
    return 1;
}

void Session::Ses_delete_memory() {
    for (auto it = save.begin(); it != save.end(); it++) {
        delete it->second;
    }
    for (auto it = save_for_delete.begin(); it != save_for_delete.end(); it++) {
        delete (*it);
    }
}

void Session::run(std::string target, int answer_num) {
    if (save[target]->calculate()) {
        std::cout << std::fixed << std::setprecision(4)
        << save[target]->value() << std::endl;
        answer[answer_num] = save[target]->value();
    }
    save[target]->reiscal();
}
