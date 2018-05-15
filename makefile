all: main.c matrix.c
	gcc main.c matrix.c -o main
w: main.c matrix.c
	gcc main.c matrix.c -o main -Wall
clean:
	rm main
