#include <stdio.h>
#include "matrix.h"


int main(int argc, char *argv[]) {
	if(argc < 5) {
		printf("Incorrect use: ./main <imp> <A> <B> C>\n");
		return 1;
	}
	switch(argv[1][0]) {
		case 'p':
			multiply(argv[4], argv[2], argv[3], &pt);
			break;
		case 'o':
			multiply(argv[4], argv[2], argv[3], &omp);
			break;
	};
	return 0;
}

