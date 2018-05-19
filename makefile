#Caio Henrique Silva Ramos - NUSP 9292991
all: main.o matrix.o
	gcc main.o matrix.o -o main -lpthread -fopenmp -O3 
matrix.o: matrix.c
	gcc matrix.c -c -O3 -Wno-unused-result
main.o: main.c
	gcc main.c -c -O3
clean:
	rm main *.o
