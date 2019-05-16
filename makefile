all: main

main: main.o Input.o Node.o Input.h Node.h
	g++ -o main main.o Input.o Node.o -std=c++14 -Wall

main.o: main.cpp Input.h Node.h
	g++ -o main.o -c main.cpp

Input.o: Input.cpp Input.h Node.h
	g++ -o Input.o -c Input.cpp

Node.o: Node.cpp Node.h
	g++ -o Node.o -c Node.cpp

clean_w:
	del *.exe *.o

clean_l:
	rm *.exe *.o

debug: main.cpp Input.cpp Node.cpp Input.h Node.h
	g++ -d debug main.cpp Input.cpp Node.cpp -std=c++11 -o main -Wall

check:
	python2 cpplint.py --filter=-build/header_guard,-build/include,-legal/copyright main.cpp Input.cpp Input.h Node.cpp Node.h 