#include <stdio.h>
#include "matrix.h"


int main(int argc, char *argv[]) {
	if(argc < 5) {
		printf("Incorrect use.\n\tUsage: ./main <imp> <A> <B> C>\n");
		return 1;
	}
	switch(argv[1][0]) {
		case 'p':
			printf("Chosen alg: Pthreads.\n");
			multiply(argv[4], argv[2], argv[3], &pt);
			break;
		case 'o':
			printf("Chosen alg: OpenMP.\n");
			multiply(argv[4], argv[2], argv[3], &omp);
			break;
		default:
			printf("%c is an invalid option\n", argv[1][0]);
	};
	return 0;
}

