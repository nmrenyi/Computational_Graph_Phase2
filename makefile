all: main1 main2 main3 main4
# main1 代表能满足实现第一阶段功能的可执行程序
# main2 代表能满足实现第二阶段功能的可执行程序
# main3 牛顿迭代法
# main4 代表能满足实现最小二乘法功能的可执行程序
# main5 代表session试验版本
# g++ main.cpp Input.cpp Node.cpp -o main1 -std=c++14 -Wall

main1: main.o Input.o Node.o Input.h Node.h
	g++ -o main1 main.o Input.o Node.o -std=c++14 -Wall

main2: main.o Input.o Node.o Input.h Node.h
	g++ -D DEBUG -o main2 main.o Input.o Node.o -std=c++14 -Wall

main3: mainNewton.o newtonMethod.o Node.o Node.h
	g++ -o main3 mainNewton.o newtonMethod.o Node.o -std=c++14 -Wall

main5: Ses_main.cpp Session.cpp Node.o Session.h Input.h Node.h
	g++ -o main5 Ses_main.cpp Session.cpp Node.o -std=c++14 -Wall

main.o: main.cpp Input.h Node.h
	g++ -o main.o -c main.cpp -Wall

Input.o: Input.cpp Input.h Node.h
	g++ -o Input.o -c Input.cpp -Wall

Node.o: Node.cpp Node.h
	g++ -o Node.o -c Node.cpp -Wall

mainNewton.o: mainNewton.cpp Node.h
	g++ -o mainNewton.o -c mainNewton.cpp -Wall

newtonMethod.o: newtonMethod.cpp Node.h
	g++ -o newtonMethod.o -c newtonMethod.cpp -Wall

mainTensor.o: mainTensor.cpp Tensor.h
	g++ -c mainTensor.cpp -Wall

Tensor.o: Tensor.cpp Tensor.h
	g++ -c Tensor.cpp -Wall

clean_w:
	del *.exe *.o

clean:
	rm main1 main2 main3 *.o

debug: main.cpp Input.cpp Node.cpp Input.h Node.h
	g++ -d debug main.cpp Input.cpp Node.cpp -std=c++11 -o main -Wall

check:
	python2 cpplint.py --filter=-build/header_guard,-build/include,-legal/copyright main.cpp Input.cpp Input.h Node.cpp Node.h newtonMethod.cpp mainNewton.cpp