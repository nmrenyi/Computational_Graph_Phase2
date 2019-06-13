#pragma once
#include <vector>
#include <map>
#include <string>

class Tensor {
 protected:
    std::vector<Tensor*> input;  // 当前节点所依赖的节点信息
    std::vector<double> data;  // 储存Tensor内部的所有值
    std::vector<int>dim;  // 每一维的大小
    bool calculated = 0;  // 之前是否计算过
 public:
    Tensor();
    std::vector<int> getDim() const;  // 返回储存维数的vector
    void initialize();  // 使节点都定义为未调用
    virtual bool calculate() = 0;  // 是否可算,并且在此过程中进行计算
    void display() const;  // 输出Tensor
    void setData(std::vector<double>);  // 设置Data
    std::vector<double> getData() const;  // 获取Data
    virtual ~Tensor();
};

class TensorConstant : public Tensor {
 private:
      // const std::vector<double> data;  // 储存Tensor内部的所有值
      // const std::vector<int>dim;  // 每一维的大小
 public:
    TensorConstant(std::vector<int>dimInfo, std::vector<double>dataInfo);
    void setData() = delete;
    bool calculate();
    // 未能成功实现Constant对象，原因是这些重写的函数无法获得自己内部的Constant

      // void setData(std::vector<double>) = delete;
      // std::vector<double> getData();
      // std::vector<int> getDim();
};

class TensorPlaceholder : public Tensor {
 public:
    explicit TensorPlaceholder(std::vector<int>);  // cpplint提示加入explict
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
    TensorSingleOperation(std::string target,
      std::string operatorName, std::map<std::string, Tensor*>& save);
    bool calculate();
};

class TensorBinaryOperation : public Tensor {
 private:
    std::string operationName;
 public:
    TensorBinaryOperation(std::string target1,
      std::string target2, std::string operatorName,
      std::vector<int>dimInfo, std::map<std::string, Tensor*>& save);
    bool calculate();
};

class TensorReshapeOperation : public Tensor {
 public:
    TensorReshapeOperation(std::string target, std::vector<int> aimDim, std::map<std::string, Tensor*>& save);
    bool calculate();
};

class TensorConcatOperation : public Tensor {
 private:
    int concatWay;
 public:
    TensorConcatOperation(std::string target1, 
      std::string target2, int concatway, std::map<std::string, Tensor*>& save);
    bool calculate();
};
