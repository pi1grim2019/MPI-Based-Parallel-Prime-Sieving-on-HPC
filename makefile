main: func_call.c include.h main.c makefile sieve0.c sieve1.c sieve2.c sieve3.c
	rm -f main
	gcc main.c -o main -I /home/act-software/mpich-3.2.1/include -L /home/act-software/mpich-3.2.1/lib -lmpi -lm -march=native -O3 