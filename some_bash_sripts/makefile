main.o : main.c
	gcc -c main.c -o main.o

cat.o : cat/cat.c
	gcc -c cat/cat.c -o cat.o 

cp.o : cp/cp.c
	gcc -c cp/cp.c -o cp.o 

echo.o : echo/echo.c
	gcc -c echo/echo.c -o echo.o 

ls.o : ls/ls.c 
	gcc -c ls/ls.c -o ls.o

pwd.o : pwd/pwd.c 
	gcc -c pwd/pwd.c -o pwd.o

wc.o : wc/wc.c 
	gcc -c wc/wc.c -o wc.o 

task: main.o cat.o cp.o echo.o ls.o pwd.o wc.o
	gcc main.o cat.o cp.o echo.o ls.o pwd.o wc.o -o exec





