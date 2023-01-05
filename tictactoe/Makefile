all: ttt libtt

ttt: ttt.o libtt
	gcc -o tictactoe ttt.c -ltt -L. -lcurses

libtt: libtt.c libtt.h
	gcc -fPIC -shared -o libtt.so libtt.c -lcurses