all:
	gcc -g -Wall -std=c99 -o game-of-life.out aaltogame.c aaltogame.h main.c
clean:
	rm -rf *.out|*.gch