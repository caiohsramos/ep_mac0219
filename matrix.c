//Caio Henrique Silva Ramos - NUSP 9292991
//Some parts of the code are commented because they were used
//	as tests or result of correctness
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <omp.h>
#include "matrix.h"

void multiply(char *c_file, char *a_file, char *b_file, double(*f_mul)(double**,double**,double**,long int,long int,long int)) {
	long int m, n, p;	
	long int i;
	double **a = read_matrix(a_file, &m, &p);
	double **b = read_matrix(b_file, &p, &n);
	double t;
	
	//allocs matrix c
	/*double **c = (double **)malloc(sizeof(double*)*m);*/
	/*for(i = 0; i < m; i++) {*/
		/*c[i] = (double*)malloc(sizeof(double)*n);*/
	/*}*/

	//regular matrix mult
	/*zeros(c, m, n);*/
	/*t = regular(c, a, b, m, p, n);*/
	/*printf("Time taken by regular: %lf\n", t);*/
	

	//temporary
	double **c_aux = (double **)malloc(sizeof(double*)*m);
	for(i = 0; i < m; i++) {
		c_aux[i] = (double*)malloc(sizeof(double)*n);
	}	

	//optimized matrix mult
	zeros(c_aux, m, n);
	if((m > 30) || (n > 30) || (p > 30)) 
		t = (*f_mul)(c_aux, a, b, m, p, n);
	else {
		printf("Small size, using regular algorithm\n");
		t = regular(c_aux, a, b, m, p, n);
	}
			
	printf("Time taken by optimization: %lf\n", t);

	/*printf("Result of correctness %ld\n", compare_matrix(c, c_aux, m, n));*/

	write_matrix(c_file, c_aux, m, n);
	free_matrix(a, m);
	free(a);
	free_matrix(b, p);
	free(b);
	/*free_matrix(c, m);*/
	/*free(c);*/
	
	//temporary
	free_matrix(c_aux, m);
	free(c_aux);
}

long int compare_matrix(double **a, double **b, long int m, long int n) {
	long int i, j;
	for(i = 0; i < m; i++)
		for(j = 0; j < n; j++)
			if(a[i][j] != b[i][j])
				return 0;	
	return 1;
}

void print_matrix(double **mat, long int m, long int n) {
	long int i, j;
	for(i = 0; i < m; i++) {
		for(j = 0; j < n; j++)
			printf("%.2lf ", mat[i][j]);
		printf("\n");
	}
	printf("\n");
}

double **read_matrix(char *file_name, long int *row, long int *col) {
	FILE *fp = NULL;
	long int i, j;
	double val;
	fp = fopen(file_name, "r+");	
	if(fp == NULL) {
		printf("Could not open file %s\n", file_name);
		exit(1);
	}	
	fscanf(fp, "%ld", row);
	fscanf(fp, "%ld", col);
	double **m = (double **)malloc(sizeof(double*)*(*row));
	for(i = 0; i < (*row); i++) {
		m[i] = (double*)malloc(sizeof(double)*(*col));
	}
	zeros(m, *row, *col);
	while((fscanf(fp, "%ld%ld%lf", &i, &j, &val)) != EOF) {
		m[i-1][j-1] = val;		
	}

	fclose(fp);
	return m;
}

void zeros(double **mat, long int m, long int n) {
	long int i, j;
	for(i = 0; i < m; i++)
		for(j = 0; j < n; j++)
			mat[i][j] = 0.f;
}

void free_matrix(double **mat, long int row) {
	long int i;
	for(i = 0; i < row; i++)
		free(mat[i]);
}

void write_matrix(char *file_name, double **c, long int row, long int col) {
	long int i, j;
	FILE *fp = NULL;
	fp = fopen(file_name, "w+");
	if(fp == NULL) {
		printf("Could not open file: %s\n", file_name);
		exit(0);
	}	
	fprintf(fp, "%ld %ld\n", row, col);

	for(i = 0; i < row; i++)
		for(j = 0; j < col; j++)
			if(c[i][j] != 0)
				fprintf(fp, "%ld %ld %lf\n", i+1, j+1, c[i][j]);
	fclose(fp);
}

void transpose(double **b_t, double **b, long int m, long int n) {
	long int i, j;
	for(i = 0; i < m; i++) 
		for(j = 0; j < n; j++)
			b_t[j][i] = b[i][j];
}

void *thread_mult(void *arg) {
	ARGS *t_arg = (ARGS*)arg;	
	long int m, n, p, id;
	double **a = t_arg->a;
	double **b = t_arg->b;
	double **c = t_arg->c;
	m = t_arg->m;
	n = t_arg->n;
	p = t_arg->p;
	id = t_arg->t_id;

	long int i, j, k;
	long int	new_m;
	long int start_i;
	//do the calculation of limits
	start_i = (m/4)*id;
	if(id != 3) new_m = (m/4)*(id+1);
	else new_m = m;

	printf("id: %ld, start_i: %ld\n", id, start_i);
	printf("id: %ld, new_m: %ld\n", id, new_m);

	for(i = start_i; i < new_m; i++) {
		for(j = 0; j < n; j++) {
			for(k = 0; k < p; k++) {
				//c[i][j] += a[i][k] * b_t[j][k];	
				//c[i][j] += a[i][k] * b[k][j];	
				//
				c[i][j] += a[i][k] * b[j][k];	
			}
		}
	}
	
}	

double pt(double **c, double **a, double **b, long int m, long int p, long int n) {
	long int i;
	double wtime;
	pthread_t threads[4];
	ARGS t_args[4];
	double **b_t = (double**)malloc(sizeof(double*)*n);
	for(i = 0; i < n; i++) {
		b_t[i] = (double*)malloc(sizeof(double)*p);
	}
	transpose(b_t, b, p, n);

	wtime = omp_get_wtime();
	for(i = 0; i < 4; i++) {
		t_args[i].t_id = i;	
		t_args[i].a = a;	
		t_args[i].b = b_t;	
		t_args[i].c = c;	
		t_args[i].m = m;	
		t_args[i].n = n;	
		t_args[i].p = p;	
		pthread_create(&threads[i], NULL, thread_mult, &t_args[i]);
	}	
	
	for(i = 0; i < 4; i++) 
		pthread_join(threads[i], NULL);

	wtime = omp_get_wtime() - wtime;
	free_matrix(b_t, n);
	free(b_t);	

	return wtime;
}



double omp(double **c, double **a, double **b, long int m, long int p, long int n) {
	double t, wtime, sum;
	
	//transpose matrix b here .....
	double **b_t = (double**)malloc(sizeof(double*)*n);
	long int i;
	for(i = 0; i < n; i++) {
		b_t[i] = (double*)malloc(sizeof(double)*p);
	}
	transpose(b_t, b, p, n);
	
	wtime = omp_get_wtime();
	#pragma omp parallel
	{
		long int i, j, k;
		//omp matrix mutiplication
		#pragma omp for
		for(i = 0; i < m; i++) {
			for(j = 0; j < n; j++) {
				for(k = 0; k < p; k++) {
					c[i][j] += a[i][k] * b_t[j][k];	
					//c[i][j] += a[i][k] * b[k][j];	
				}
			}
		}
	}
	wtime = omp_get_wtime() - wtime;
	free_matrix(b_t, n);
	free(b_t);
	return wtime;

}

double regular(double **c, double **a, double **b, long int m, long int p, long int n) {
	long int i, j, k;
	double wtime;
	//transpose matrix b here .....
	double **b_t = (double**)malloc(sizeof(double*)*n);
	for(i = 0; i < n; i++) {
		b_t[i] = (double*)malloc(sizeof(double)*p);
	}
	transpose(b_t, b, p, n);
	wtime = omp_get_wtime();
	//regular matrix mutiplication
	for(i = 0; i < m; i++) {
		for(j = 0; j < n; j++) {
			for(k = 0; k < p; k++) {
				/*c[i][j] += a[i][k] * b[k][j];	*/
				c[i][j] += a[i][k] * b_t[j][k];	
			}
		}
	}
	wtime = omp_get_wtime() - wtime;
	free_matrix(b_t, n);
	free(b_t);
	return wtime;

}

