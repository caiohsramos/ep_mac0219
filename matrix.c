#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"


void multiply(char *c_file, char *a_file, char *b_file, double**(*f_mul)(double**,double**,int,int,int)) {
	int m, n, p;	
	double **a = read_matrix(a_file, &m, &p);
	double **b = read_matrix(b_file, &p, &n);


	//double **c = (*f_mul)(a, b, m, p, n); 

	printf("Matrix A:\n");
	print_matrix(a, m, p);
	printf("Matrix B:\n");
	print_matrix(b, p, n);



	//write_matrix(c_file, c, m, n);
	free_matrix(a, m);
	free_matrix(b, p);
	//free_matrix(c, m);
}

void print_matrix(double **mat, int m, int n) {
	int i, j;
	for(i = 0; i < m; i++) {
		for(j = 0; j < n; j++)
			printf("%.2lf ", mat[i][j]);
		printf("\n");
	}
	printf("\n");
}

double **read_matrix(char *file_name, int *row, int *col) {
	FILE *fp = NULL;
	int i, j;
	double val;
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
	while((fscanf(fp, "%d%d%lf", &i, &j, &val)) != EOF) {
		m[i][j] = val;		
	}

	fclose(fp);
	return m;
}

void zeros(double **mat, int m, int n) {
	int i, j;
	for(i = 0; i < m; i++)
		for(j = 0; j < n; j++)
			mat[i][j] = 0.f;
}

void free_matrix(double **mat, int row) {
	int i;
	for(i = 0; i < row; i++)
		free(mat[i]);
}

void write_matrix(char *file_name, double **c, int row, int col) {
	int i, j;
	FILE *fp = NULL;
	fp = fopen(file_name, "w+");
	if(fp == NULL) {
		printf("Could not open file: %s\n", file_name);
		exit(0);
	}	
	fprintf(fp, "%d %d\n", row, col);

	for(i = 0; i < row; i++)
		for(j = 0; j < col; j++)
			if(c[i][j] != 0)
				fprintf(fp, "%d %d %lf\n", i, j, c[i][j]);
	fclose(fp);
}

double **pt(double **a, double **b, int m, int p, int n) {

}

double **omp(double **a, double **b, int m, int p, int n) {

}

