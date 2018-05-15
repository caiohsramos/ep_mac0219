#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"


void multiply(char *c_file, char *a_file, char *b_file, double**(*f_mul)(double**,double**,int,int,int)) {
	int m, n, p;	
	double **a = read_matrix(a_file, &m, &p);
	double **b = read_matrix(a_file, &p, &n);


	double **c = (*f_mul)(a, b, m, p, n); 



	write_matrix(c_file, c, m, n);
	//free a, b and c
}

double **read_matrix(char *file_name, int *row, int *col) {
	FILE *fp = NULL;
	int i, j, n;
	fp = fopen(file_name, "r+");	
	if(fp == NULL) {
		printf("Could not open file %s\n", file_name);
		exit(1);
	}	
	fscanf(fp, "%d", row);
	fscanf(fp, "%d", col);
	double **m = (double **)malloc(sizeof(double*)*(*row));
	for(i = 0; i < (*row); i++) {
		m[i] = (double*)malloc(sizeof(double)*(*col));
	}
	zeros(m, *row, *col);
	while((fscanf(fp, "%d%d%d", &i, &j, &n)) != EOF) {
		
	}

	fclose(fp);
}

void zeros(double **mat, int m, int n) {
	int i, j;
	for(i = 0; i < m; i++)
		for(j = 0; j < n; j++)
			mat[i][j] = 0.f;
}


void write_matrix(char *file_name, double **c, int row, int col) {
	
}

double **pt(double **a, double **b, int m, int p, int n) {

}

double **omp(double **a, double **b, int m, int p, int n) {

}
