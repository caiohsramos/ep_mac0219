#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <omp.h>
#include "matrix.h"

void multiply(char *c_file, char *a_file, char *b_file, double(*f_mul)(double**,double**,double**,int,int,int)) {
	int m, n, p;	
	int i;
	double **a = read_matrix(a_file, &m, &p);
	double **b = read_matrix(b_file, &p, &n);
	double t;



	//allocs matrix c
	/*double **c = (double **)malloc(sizeof(double*)*m);*/
	/*for(i = 0; i < m; i++) {*/
		/*c[i] = (double*)malloc(sizeof(double)*n);*/
	/*}*/

	/*regular matrix mult
	zeros(c, m, n);
	t = regular(c, a, b, m, p, n);
	printf("Time taken by regular: %lf\n", t);
	*/

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

	//printf("Result of correctness %d\n", compare_matrix(c, c_aux, m, n));

	write_matrix(c_file, c_aux, m, n);
	free_matrix(a, m);
	free(a);
	free_matrix(b, p);
	free(b);
	//free_matrix(c, m);
	//free(c);
	
	//temporary
	free_matrix(c_aux, m);
	free(c_aux);
}

int compare_matrix(double **a, double **b, int m, int n) {
	int i, j;
	for(i = 0; i < m; i++)
		for(j = 0; j < n; j++)
			if(a[i][j] != b[i][j])
				return 0;	
	return 1;
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
		m[i-1][j-1] = val;		
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
				fprintf(fp, "%d %d %lf\n", i+1, j+1, c[i][j]);
	fclose(fp);
}

void transpose(double **b_t, double **b, int m, int n) {
	int i, j;
	for(i = 0; i < m; i++) 
		for(j = 0; j < n; j++)
			b_t[j][i] = b[i][j];
}

void *thread_mult(void *arg) {
	ARGS *t_arg = (ARGS*)arg;	
	int m, n, p, id;
	double **a = t_arg->a;
	double **b = t_arg->b;
	double **c = t_arg->c;
	m = t_arg->m;
	n = t_arg->n;
	p = t_arg->p;
	id = t_arg->t_id;

	int i, j, k;
	int	new_m;
	int start_i;
	//do the calculation of limits
	start_i = (m/4)*id;
	new_m = (m/4)*(id+1);

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

double pt(double **c, double **a, double **b, int m, int p, int n) {
	int i;
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



double omp(double **c, double **a, double **b, int m, int p, int n) {
	double t, wtime, sum;
	
	//transpose matrix b here .....
	double **b_t = (double**)malloc(sizeof(double*)*n);
	int i;
	for(i = 0; i < n; i++) {
		b_t[i] = (double*)malloc(sizeof(double)*p);
	}
	transpose(b_t, b, p, n);
	
	wtime = omp_get_wtime();
	#pragma omp parallel
	{
		int i, j, k;
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

double regular(double **c, double **a, double **b, int m, int p, int n) {
	int i, j, k;
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

