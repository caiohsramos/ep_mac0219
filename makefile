all: main.c matrix.c
	gcc main.c matrix.c -o main
clean:
	rm main
