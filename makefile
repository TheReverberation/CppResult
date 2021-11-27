all: main

main: main.cpp
	clang++ main.cpp -std=c++17 -o main
