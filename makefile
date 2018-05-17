all: main.c matrix.c
	gcc main.c matrix.c -o main -lm -lpthread -fopenmp -O3
clean:
	rm main
