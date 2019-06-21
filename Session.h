#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "Node.h"
#include "Input.h"
#include "Node.h"

class Session {
public:
    std::stringstream in;
    std::map <std::string, base*> save;
    std::map <int, double> answer;
    std::vector <base*> save_for_delete;
    Session();
    Session(std::stringstream& ss);
    ~Session();
    void Make_Graph();
    void Ses_InputNode();
    void Ses_Inputoperate();
    int Ses_Inputevalnum(int answer_num);
    void Ses_delete_memory();

    void run(std::string, int);
};
