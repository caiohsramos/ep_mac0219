//Caio Henrique Silva Ramos - NUSP 9292991
#ifndef __MATRIX_C__
#define __MATRIX_C__

void multiply(char *c_file, char *a_file, char *b_file, double(*f_mul)(double**,double**,double**,long int,long int,long int));
void write_matrix(char*, double**, long int, long int);
double **read_matrix(char*, long int*, long int*);
double pt     (double **c, double **a, double **b, long int m, long int p, long int n); 
double omp    (double **c, double **a, double **b, long int m, long int p, long int n);
double regular(double **c, double **a, double **b, long int m, long int p, long int n);
void zeros(double **, long int, long int);
void free_matrix(double **, long int );
void print_matrix(double **, long int, long int);
void transpose(double **, double **, long int , long int );
void *thread_mult(void *);
long int compare_matrix(double **, double **, long int , long int );

typedef struct {
	long int t_id;
	double **c;
	double **a;
	double **b;
	long int m;
	long int n;
	long int p;
} ARGS;


#endif
