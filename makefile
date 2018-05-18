all: main.c matrix.c
	gcc main.c matrix.c -o main -lpthread -fopenmp -O3 -Wno-unused-result
clean:
	rm main
