#include <vector>
#include <map>
#include <string>
class Tensor {
 protected:
    std::vector<Tensor*> input;
    std::vector<double> data;  // 储存Tensor内部的所有值
    std::vector<int>dim;  // 每一维的大小
 public:
    Tensor(){}
    virtual ~Tensor() {};
};

class TensorConstant : public Tensor {
 private:
    const std::vector<double> data;  // 储存Tensor内部的所有值
    const std::vector<int>dim;  // 每一维的大小
 public:
    TensorConstant(std::vector<int>dimInfo, std::vector<double>dataInfo) : dim(dimInfo), data(dataInfo) {}
    
};

class TensorPlaceholder : public Tensor {
 public:
    TensorPlaceholder(std::vector<int>dimInfo) {
        dim = dimInfo;
    }
};

class TensorVariable : public Tensor {
 public:
    TensorVariable(std::vector<int>dimInfo, std::vector<double>dataInfo) {
        dim = dimInfo;
        data = dataInfo;
    }
};

class TensorSingleOperation : public Tensor {
 private:
    std::string operationName;
 public:
    TensorSingleOperation(std::string target, std::string operatorName, std::map<std::string, Tensor*>& save) {
        operationName = operatorName;
        input.push_back(save[target]);
    }
};

class TensorBinaryOperation : public Tensor {
 private:
    std::string operationName;
 public:
    TensorBinaryOperation(std::string target1, std::string target2, std::string operatorName, std::map<std::string, Tensor*>& save) {
        operationName = operatorName;
        input.push_back(save[target1]);
        input.push_back(save[target2]);
    }
};
