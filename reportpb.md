## 一、结构/封装/接口
### 1、第一阶段的结构
对于我们继续开发的其他组的代码，我们没有进行结构和功能上的更改。第一阶段结构有一个基类Base类，里面有当前节点的数据以及一些其他操作的函数，有Placeholder/Constant/Variable类继承Base类，也有SingleOperation和BinaryOperation继承Base类实现对应的功能。
### 2、第二阶段的结构
### 基础功能
+  (1) 牛顿迭代法的实现在mainNewton.cpp和newtonMethod.cpp当中，可单独编译生成main4可执行文件。mainNewton.cpp主要负责将每个方程读入，newtonMethod.cpp负责给每个方程建图、求导并且输出结果。其中的Counter类实现了通过单例模式进行计数，体现了OOP特性。其他函数的功能详见注释。
+  (2) ASSIGN和BIND的实现在_Node.h和Node.cpp中给出，规范地实现了题目要求的功能，具体实现过程详见代码及注释。
### 扩展功能
+ (1) 有关Tensor的全部功能在_Tensor.h当中类的定义中有体现。Tensor类储存着Tensor的具体信息，其余的类继承了Tensor类，实现了特定的功能。Tensor类的数据成员均为protected，方便了派生类成员函数访问，也保证了封装性。
+ (2) Reshape功能在_Tensor.h和Tensor.cpp中给出，用以改变Tensor的形状(维数及各维数据量)，但不改变Tensor中的数据。
+ (3) Concat功能在_Tensor.h和Tensor.cpp中给出，用以将两个符合一定要求(详见代码及注释)的Tensor按照不同方式(以不同维数为轴)捏合成一个Tensor。


## 二、功能展示
### 基础功能
+ 1. 完成了牛顿迭代法的要求。**优点**在于：通过递归的方法，将方程求导问题转化为了已知的计算图求导问题，代码复用率较高，体现了将未知问题转化为已知问题解决的思想。**可改进之处**也存在，例如x^n建图需要2n-2个节点，效率略低，可考虑快速幂或者新建幂运算节点进行改进。

### 扩展功能
+ 1. 实现了Tensor类的计算图，支持Placeholder/Constant/Parametre等，支持加减乘运算。矩阵运算时可以匹配大小，不匹配时可以检查能否broadcast或者报错。**优点**在于实现了broadcast/reshape/concat的功能，使Tensor运算图较为完善。**可改进之处**也存在，例如受到第一阶段代码框架可扩展性的影响，Tensor的运算和broadcast等操作只实现到二维矩阵。
```
样例输入
3
x P 2 1 2
y C 2 2 2 1 2 3 4
z V 1 1 10
3
a = x + y
b = z - x
c = y * y
3
EVAL a 1 x 4 5
EVAL b 1 x 9 10
EVAL c

样例输出
5 7 8 9
1 0 1 0
1 4 9 16
```
+ 2. Reshape功能在_Tensor.h和Tensor.cpp中给出，用以改变Tensor的形状(维数及各维数据量)，但不改变Tensor中的数据。Reshape类结点的输入在第二阶段的输入中进行，输入规范如下：  
第一行输入5个字串,其中第五个表示将Tensor经过Reshape变换成的维数，如下表示a为将b转换成3维的Tensor：
```
a = RESHAPE b 3
```

第二行输入各维的数据数量，如：
```
2 2 2
```
则a将8个数据的Tensor b转换成2*2*2的Tensor，具体效果见示例代码。  
```
1
a C 2
2 4
1 2 3 4 5 6 7 8
2
b = RESHAPE a 3
2 2 2
c = RESHAPE a c
4 2
2
EVAL b
EVAL c
```
+ 3.  Concat功能在_Tensor.h和Tensor.cpp中给出，用以将两个符合一定要求(详见代码及注释)的Tensor按照不同方式(以不同维数为轴)捏合成一个Tensor。  Concat类结点的输入在第二阶段中进行，输入规范如下：  
输入6个字符串，第4/5个为要Concat的两个Tensor的名称，第6个为Concat的方式，即第六个字串为n，则以第n+1个维度为轴将两个矩阵捏合。如：
```
a = CONCAT b c 0
```
为按第一个维度捏合b、c两个Tensor.具体效果见示例代码。  

```
2
a C 2
2 2
1 2 3 4
b C 2
2 2
5 6 7 8
2
c = CONCAT a b 0
d = CONCAT a b 1
2
EVAL c
EVAL d
```