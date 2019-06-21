#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "_Node.h"
#include "_Input.h"

class Session {
public:
    std::stringstream in; // 保存文件建图的字符串
    std::map <std::string, base*> save;
    std::map <int, double> answer;
    std::vector <base*> save_for_delete;
    Session();
    Session(std::stringstream& ss);
    ~Session();
    void Make_Graph(); // 从in建图
    void Ses_InputNode();
    void Ses_Inputoperate();
    int Ses_Inputevalnum(int answer_num);
    void Ses_delete_memory();

    void run(std::string, int); // 实现每个session的EVAL
};
