lex.o: LEX/lex.cpp
	g++ -g -c LEX/lex.cpp -o lex.o

id.o: ID_TID/identificator.cpp
	g++ -g -c ID_TID/identificator.cpp -o id.o

pars.o: PARSER/parser.cpp
	g++ -g -c PARSER/parser.cpp -o pars.o

exec.o: EXECUTOR/executor.cpp
	g++ -c EXECUTOR/executor.cpp -o exec.o

main.o: main.cpp
	g++ -g -c main.cpp -o main.o

task: main.o id.o lex.o pars.o exec.o
	g++ -g main.o id.o lex.o pars.o exec.o -o task
	rm *.o

invalid_tests: task
	./task TESTS/invalid/inval1.txt
	./task TESTS/invalid/inval2.txt
	./task TESTS/invalid/inval3.txt
	./task TESTS/invalid/inval4.txt

test1: task
	./task TESTS/valid/test1.txt

test2: task
	./task TESTS/valid/test2.txt

test3: task
	./task TESTS/valid/test3.txt

test4: task
	./task TESTS/valid/test4.txt

test5: task
	./task TESTS/valid/test5.txt

myprog: task
	./task my_program.txt


clean:
	rm -r *.o task