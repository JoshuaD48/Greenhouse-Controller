#makefile
lab02:  lab02.o ghcontrol.o
		gcc -g -o lab02 lab02.o ghcontrol.o
lab02.o: lab02.c ghcontrol.h
		gcc -g -c lab02.c
ghcontrol.o: ghcontrol.c ghcontrol.h
	gcc -g -c ghcontrol.c

