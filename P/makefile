task: Polynomial.cpp main.cpp
	g++ -o task Polynomial.cpp main.cpp -w

run: task
	valgrind ./task c
	valgrind ./task x^4-2x^2-x+1 x^2+2x+1 
	valgrind ./task 10x^2-3x 0

	


clean:
	rm -rf *.o task
