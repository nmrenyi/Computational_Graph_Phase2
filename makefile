main: main.cpp Input.cpp Node.cpp Input.h Node.h
	g++ main.cpp Input.cpp Node.cpp -std=c++11 -o main
check:
	python2 cpplint.py --filter=-build/header_guard,-build/include,-legal/copyright main.cpp Input.cpp Input.h Node.cpp Node.h 