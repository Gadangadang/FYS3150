all: compile execute

compile:
	g++ -std=c++11 -O3 -o main.x main.cpp  -larmadillo -llapack -lblas

execute:
	./main.x 8
	python3 Plotte_code.py
