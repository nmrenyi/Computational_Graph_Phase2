all: main1 main2 main3 main4 main5
# main1 代表能满足实现第一阶段功能的可执行程序
# main2 代表能满足实现第二阶段功能的可执行程序
# main3 牛顿迭代法
# main4 代表能满足实现最小二乘法功能的可执行程序
# main5 代表session试验版本
# g++ main.cpp Input.cpp Node.cpp -o main1 -std=c++14 -Wall

main1: main.o Input.o Node.o _Input.h _Node.h
	g++ -o main1 main.o Input.o Node.o -std=c++14 -Wall

main2: main.o Input.o Node.o _Input.h _Node.h
	g++ -o main2 main.o Input.o Node.o -std=c++14 -Wall

main3: mainNewton.o newtonMethod.o Node.o _Node.h
	g++ -o main3 mainNewton.o newtonMethod.o Node.o -std=c++14 -Wall

main4: mainTensor.o Tensor.o TensorInput.o _Tensor.h
	g++ mainTensor.o Tensor.o TensorInput.o -o main4

main5: Ses_main.cpp Session.cpp Node.o Session.h _Input.h _Node.h
	g++ -o main5 Ses_main.cpp Session.cpp Node.o -std=c++14 -Wall

main4d: mainTensor.cpp Tensor.cpp TensorInput.cpp _Tensor.h
	g++ mainTensor.cpp Tensor.cpp TensorInput.cpp -o main4d -D debug -Wall

main.o: main.cpp _Input.h _Node.h
	g++ -o main.o -c main.cpp -Wall

Input.o: Input.cpp _Input.h _Node.h
	g++ -o Input.o -c Input.cpp -Wall

Node.o: Node.cpp _Node.h
	g++ -o Node.o -c Node.cpp -Wall

mainNewton.o: mainNewton.cpp _Node.h
	g++ -o mainNewton.o -c mainNewton.cpp -Wall

newtonMethod.o: newtonMethod.cpp _Node.h
	g++ -o newtonMethod.o -c newtonMethod.cpp -Wall

mainTensor.o: mainTensor.cpp _Tensor.h
	g++ -c mainTensor.cpp -Wall

TensorInput.o: TensorInput.cpp _Tensor.h
	g++ -c TensorInput.cpp -Wall

Tensor.o: Tensor.cpp _Tensor.h
	g++ -c Tensor.cpp -Wall

clean_w:
	del *.exe *.o

clean:
	rm main1 main2 main3 main4 main5 *.o

debug: main.cpp Input.cpp Node.cpp _Input.h _Node.h
	g++ -d debug main.cpp Input.cpp Node.cpp -std=c++11 -o main -Wall

# python 3.x 版本可运行下面的命令
check:
	python cpplint.py --filter=-build/include,-legal/copyright,-runtime/references main.cpp Input.cpp _Input.h Node.cpp _Node.h newtonMethod.cpp mainNewton.cpp mainTensor.cpp Tensor.cpp TensorInput.cpp _Tensor.h