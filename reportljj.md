# OOP大作业：计算图
## **一、结构/封装/接口**
### **1、第一阶段的结构**
对于我们继续开发的其他组的代码，我们没有进行结构和功能上的更改。第一阶段结构有一个基类Base类，里面有当前节点的数据以及一些其他操作的函数，有Placeholder/Constant/Variable类继承Base类，也有SingleOperation和BinaryOperation继承Base类实现对应的功能。

### **2、第二阶段的结构**
### **基础功能**
1. 牛顿迭代法的实现在mainNewton.cpp和newtonMethod.cpp当中，可单独编译生成main4可执行文件。mainNewton.cpp主要负责将每个方程读入，newtonMethod.cpp负责给每个方程建图、求导并且输出结果。其中的Counter类实现了通过单例模式进行计数，体现了OOP特性。其他函数的功能详见注释。
   
2. 实现链式求导功能：包含在Node.cpp中，对于每个节点，我们写了derivate函数，从根节点开始往下求导。为了保证求导的正确性，在每一次求导前我们会进行一次calculate，同时完成对每个节点前驱数目的记录。求导时，判断当前是否已经遍历过了所有的前驱，若是则往下继续反向传播求导。

### **扩展功能**
1. 有关Tensor的全部功能在_Tensor.h当中类的定义中有体现。Tensor类储存着Tensor的具体信息，其余的类继承了Tensor类，实现了特定的功能。Tensor类的数据成员均为protected，方便了派生类成员函数访问，也保证了封装性。

2. 实现完整的求导功能：和基础功能实现方式完全一样，对于不能求导的特殊节点做了自定义的特殊处理。

3. 实现了session功能。对于每个session，我们采取的方式是重新建图。ses_main.cpp, session.cpp完成了所有功能。在第一次读入数据建图的时候，把这些数据以stringstream的形式保存下来以便以后建图。自定义了session的语法，使用SES命令跳转到指定的session，若不存在就直接新建。session类的接口仿照main函数的实现。
## **二、功能展示**
### **基础功能**
1. 完成了牛顿迭代法的要求。**优点**在于：通过递归的方法，将方程求导问题转化为了已知的计算图求导问题，代码复用率较高，体现了将未知问题转化为已知问题解决的思想。**可改进之处**也存在，例如x^n建图需要2n-2个节点，效率略低，可考虑快速幂或者新建幂运算节点进行改进。
   
2. 完成了基本的求导功能。**优点**在于：反向传播保证了在O(n)的时间内完成求导，记录前驱数目的做法保证了非线性运算的可复用性。**可改进之处**在于不能实现高阶导。

### **扩展功能**
1. 实现了Tensor类的计算图，支持Placeholder/Constant/Parametre等，支持加减乘运算。矩阵运算时可以匹配大小，不匹配时可以检查能否broadcast或者报错。**优点**在于实现了broadcast/reshape/concat的功能，使Tensor运算图较为完善。**可改进之处**也存在，例如受到第一阶段代码框架可扩展性的影响，Tensor的运算和broadcast等操作只实现到二维矩阵。
    + 样例输入
        ```
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
        ```

    + 样例输出
        ```
        5 7 8 9
        1 0 1 0
        1 4 9 16
        ```
2. 实现了对于非加和乘节点的求导；为所有可以求导的运算符添加求导方法，使grad()函数能够在有除+*运算符以外的运算符中也能完成自动求导。
    + 样例输入
        ```
        2
        x P
        y P
        17
        a = x - y
        fa = GRAD a
        fax = fa AT x
        fay = fa AT y
        b = x / y
        fb = GRAD b
        fbx = fb AT x
        fby = fb AT y
        c = SIN x
        fc = GRAD c
        fcx = fc AT x
        d = EXP x
        fd = GRAD d
        fdx = fd AT x
        e = LOG x
        fe = GRAD e
        fex = fe AT x
        7
        EVAL fax 2 x 2.0 y 1.0
        EVAL fay 2 x 2.0 y 1.0
        EVAL fbx 2 x 2.0 y 1.0
        EVAL fby 2 x 2.0 y 1.0
        EVAL fcx 1 x 3.14159
        EVAL fdx 1 x 1.0
        EVAL fex 1 x 2.0
        ```
    + 输出样例
        ```
        1.0000
        -1.0000
        1.0000
        -2.0000
        -1.0000
        2.7183
        0.5000
        ```
3. 实现了运算图的session功能，在EVAL时候输入0，跳转到session功能（为了兼容原来的输入），SES命令跳转或新建session，原来的EVAL可以继续使用，RUN相当于无参数的EVAL，输入EOF中止输入。
    + 输入样例
        ```
        2
        x V 1.0
        y V 1.0
        3
        a = x + y
        b = a + x
        c = b + y
        0
        EVAL a
        RUN b
        EVAL c 2 x 1.1 y 1.1
        SES 2_ses 2 x 2.0 y 2.0
        EVAL a
        EVAL b 2 x 2.2 y 2.2
        RUN c
        SES default_session 0
        RUN a
        EVAL b
        EVAL c
        SES 3_ses 2 x 3.0 y 3.0
        EVAL a
        EVAL b
        RUN c
        SES 2_ses 0
        RUN a
        EVAL b 2 x 2.3 y 2.3
        EVAL c
        EOF
        ```
    + 输出样例
        ```
        2.0000
        3.0000
        4.4000
        4.0000
        6.6000
        8.8000
        2.2000
        3.3000
        4.4000
        6.0000
        9.0000
        12.0000
        4.4000
        6.9000
        9.2000
        ```

