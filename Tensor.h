#include <vector>
#include <map>
#include <string>

class Tensor {
 protected:
    std::vector<Tensor*> input;  // 当前节点所依赖的节点信息
    std::vector<double> data;  // 储存Tensor内部的所有值
    std::vector<int>dim;  // 每一维的大小
    bool calculated = 0;
 public:
    Tensor();
    std::vector<int> getDim();
    void initialize();  // 使节点都定义为未调用       
    virtual bool calculate() = 0;  // 是否可算,并且在此过程中进行计算
    void display();
    virtual ~Tensor();
};

class TensorConstant : public Tensor {
 private:
    const std::vector<double> data;  // 储存Tensor内部的所有值
    const std::vector<int>dim;  // 每一维的大小
 public:
    TensorConstant(std::vector<int>dimInfo, std::vector<double>dataInfo);
    bool calculate();
};

class TensorPlaceholder : public Tensor {
 public:
    TensorPlaceholder(std::vector<int>dimInfo);
    bool calculate();
};

class TensorVariable : public Tensor {
 public:
    TensorVariable(std::vector<int>dimInfo, std::vector<double>dataInfo);
    bool calculate();
};

class TensorSingleOperation : public Tensor {
 private:
    std::string operationName;
 public:
    TensorSingleOperation(std::string target, std::string operatorName, std::map<std::string, Tensor*>& save);
    bool calculate();
};

class TensorBinaryOperation : public Tensor {
 private:
    std::string operationName;
 public:
    TensorBinaryOperation(std::string target1, std::string target2, std::string operatorName, std::vector<int>dimInfo, std::map<std::string, Tensor*>& save);
    bool broadcastable(); 
    bool calculate();
    bool checkBroadcastable(Tensor*, Tensor*);
};
