#include "Input.h"
#include "Node.h"
#include "Session.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

Session d_Ses;
void InputNode() {
    std::string name;
    char type;
    std::cin >> name; d_Ses.in << name << " ";
    std::cin >> type; d_Ses.in << type << " ";
    // 查找之前是否存在同名结点(指最开始的C、P、V类型)
    // 如果存在，将其push_back进save_for_delete以便后来删除而防止内存泄漏
    if (d_Ses.save.find(name) != d_Ses.save.end()) {
        d_Ses.save_for_delete.push_back(d_Ses.save[name]);
    }
    switch (type) {  //判断输入的变量的类型
        case 'P': {
            base* Newplace = new Placeholder;
            d_Ses.save[name] = Newplace;
        }
        break;
        case 'C': {
            base* Newconstant = new Constant;
            d_Ses.save[name] = Newconstant;
            double value;
            std::cin >> value; d_Ses.in << value << "\n";
            Newconstant->set(value);
        }
        break;
        case 'V': {
            base* Newvariable = new Variable;
            d_Ses.save[name] = Newvariable;
            double value;
            std::cin >> value; d_Ses.in << value << "\n";
            Newvariable->set(value);
        }
        break;
    }
}

void Inputoperate() {
    std::string str;
    std::string tmp;
    std::vector <std::string> buffer;  // 用sstream存储当前输入的关键信息
    getline(std::cin, str);
    std::stringstream ss(str);
    d_Ses.in << str << "\n";
    while (ss >> tmp) { buffer.push_back(tmp); }

    // 查找之前是否存在同名结点（指单目、双目等运算符类型）
    // 如果存在，将其push_back进save_for_delete以便后来删除而防止内存泄漏
    if (d_Ses.save.find(buffer[0]) != d_Ses.save.end()) {
        d_Ses.save_for_delete.push_back(d_Ses.save[buffer[0]]);
    }

    if (buffer[2] == "GRAD") {  // GRAD
        base* gradname = new GRAD(buffer[3], d_Ses.save);
        d_Ses.save[buffer[0]] = gradname;
    } else if (buffer[3] == "AT") {  // AT
        base* atname = new AT(buffer[2], buffer[4], d_Ses.save);
        d_Ses.save[buffer[0]] = atname;
    } else if (buffer.size() == 4) {  // 单目运算符
        base* singlename = new singleoperation(buffer[3], buffer[2], d_Ses.save);
        d_Ses.save[buffer[0]] = singlename;
    } else if (buffer.size() == 5) {  // 双目运算符
        if (buffer[2] == "BIND") {
            base* bindname =
            new bindoperation(buffer[3], buffer[4], d_Ses.save);
            d_Ses.save[buffer[0]] = bindname;
        } else if (buffer[2] == "ASSIGN") {  // ASSIGN 操作
            base* assignName =
            new AssignOperation(buffer[3], buffer[4], d_Ses.save);
            d_Ses.save[buffer[0]] = assignName;
        } else {
            base* binaryname =
            new binaryoperation(buffer[2], buffer[4], buffer[3], d_Ses.save);
            d_Ses.save[buffer[0]] = binaryname;
        }
    } else if (buffer.size() == 6) {  // COND
        base* condname =
        new COND(buffer[3], buffer[4], buffer[5], buffer[0], d_Ses.save);
        d_Ses.save[buffer[0]] = condname;
    }
}

void Inputevalnum(int answer_num) {
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
        if (buffer.size() == 2) {  //如果直接输出某结点而不赋值
            if (d_Ses.save[target]->calculate()) {
                std::cout << std::fixed << std::setprecision(4)
                << d_Ses.save[target]->value() << std::endl;
                d_Ses.answer[answer_num] = d_Ses.save[target]->value();
            }
            d_Ses.save[target]->reiscal();
            return;
        }
        int num = stoi(buffer[2]);
        for (int i = 0; i < num; i++) {
            d_Ses.save[buffer[2 * i + 3]]->set(stod(buffer[2 * i + 4]));
        }
        if (d_Ses.save[target]->calculate()) {
            std::cout << std::fixed << std::setprecision(4)
            << d_Ses.save[target]->value() << std::endl;
            d_Ses.answer[answer_num] = d_Ses.save[target]->value();
        }
        d_Ses.save[target]->reiscal();  // 刷新结点的状态
        return;
    } else if (buffer[0] == "SETCONSTANT") {
        double tmp_var = stod(buffer[2]);
        d_Ses.save[buffer[1]]->set(tmp_var);
    } else if (buffer[0] == "SETANSWER") {
        int tmp_var = stoi(buffer[2]);
        d_Ses.save[buffer[1]]->set(d_Ses.answer[tmp_var]);
    }
}


void delete_memory() {
    for (auto it = d_Ses.save.begin(); it != d_Ses.save.end(); it++) {
        delete it->second;
    }
    for (auto it = d_Ses.save_for_delete.begin(); it != d_Ses.save_for_delete.end(); it++) {
        delete (*it);
    }
}

int main() {
    int Nodenum = 0, operatenum = 0, evalnum = 0;
    std::cin >> Nodenum; d_Ses.in << Nodenum << "\n";
    for (int i = 1; i <= Nodenum; i++) {  // 构建变量结点
        InputNode();
    }
    std::cin >> operatenum; d_Ses.in << operatenum << "\n";
    getchar();  // 吸收operatenum后的换行符
    for (int i = 1; i <= operatenum; i++) {  // 构建运算符结点
        Inputoperate();
    }
    std::cin >> evalnum;
    getchar();  // 吸收evalnum后的换行符
    for (int i = 1; i <= evalnum; i++) {  //进行输出运算操作
        Inputevalnum(i);
    }
    // 开始正式的session，只有当evalnum = 0时才开始
    if(!evalnum) {
        std::map<std::string, Session*> my_ses;
        int i = 0;
        Session* Now_Ses = &d_Ses;
        my_ses["default_session"] = Now_Ses;
        while(1) {
            int check = Now_Ses->Ses_Inputevalnum(i++);
            if(!check) break;
            else if (check == 1) ;
            else if (check == 2) {
                std::string name;
                std::cin >> name;
                if(my_ses.find(name) == my_ses.end()) {
                    Now_Ses = new Session(Now_Ses->in);
                    my_ses[name] = Now_Ses;
                } else {
                    Now_Ses = my_ses[name];
                }
                i = Now_Ses->answer.size();
                int op_num; std::cin >> op_num;
                while(op_num--) {
                    // 待完成
                    std::string n; std::cin >> n;
                    double v; std::cin >> v;
                    Now_Ses->save[n]->set(v);
                }
            } else if (check == 3) { // Run();
                std::string name;
                std::cin >> name;
                Now_Ses->run(name, i++);
            }
        }
    }
    // delete_memory();  // 释放内存
    return 0;
}
