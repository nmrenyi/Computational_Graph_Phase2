all: main1 main2 main3 main4 main5
# main1 代表能满足实现第一阶段功能的可执行程序
# main2 代表能满足实现第二阶段功能的可执行程序
# main3 牛顿迭代法
# main4 代表能满足实现Tensor相关功能的可执行程序
# main5 代表session功能的可执行程序

main1: main.o Input.o Node.o _Input.h _Node.h
	g++ -o main1 main.o Input.o Node.o -std=c++14 -Wall

main2: main.o Input.o Node.o _Input.h _Node.h
	g++ -o main2 main.o Input.o Node.o -std=c++14 -Wall

main3: mainNewton.o newtonMethod.o Node.o _Node.h
	g++ -o main3 mainNewton.o newtonMethod.o Node.o -std=c++14 -Wall

main4: mainTensor.o Tensor.o TensorInput.o _Tensor.h
	g++ -o main4 mainTensor.o Tensor.o TensorInput.o

main5: sesMain.cpp session.cpp Node.o _Session.h _Input.h _Node.h
	g++ -o main5 sesMain.cpp session.cpp Node.o -std=c++14 -Wall

main.o: main.cpp _Input.h _Node.h
	g++ -o main.o -c main.cpp -Wall

Input.o: input.cpp _Input.h _Node.h
	g++ -o Input.o -c input.cpp -Wall

Node.o: node.cpp _Node.h
	g++ -o Node.o -c node.cpp -Wall

mainNewton.o: mainNewton.cpp _Node.h
	g++ -o mainNewton.o -c mainNewton.cpp -Wall

newtonMethod.o: newtonMethod.cpp _Node.h
	g++ -o newtonMethod.o -c newtonMethod.cpp -Wall

mainTensor.o: mainTensor.cpp _Tensor.h
	g++ -c mainTensor.cpp -Wall

TensorInput.o: tensorInput.cpp _Tensor.h
	g++ -c tensorInput.cpp -Wall

Tensor.o: tensor.cpp _Tensor.h
	g++ -c tensor.cpp -Wall

clean_w:
	del *.exe *.o

clean:
	rm main1 main2 main3 main4 main5 *.o

# python 3.x 版本可运行下面的命令
check:
	python cpplint.py --filter=-build/include,-legal/copyright,-runtime/references main.cpp input.cpp _Input.h node.cpp _Node.h newtonMethod.cpp mainNewton.cpp mainTensor.cpp tensor.cpp tensorInput.cpp _Tensor.h