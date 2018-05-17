#ifndef __MATRIX_C__
#define __MATRIX_C__

void multiply(char *c_file, char *a_file, char *b_file, double(*f_mul)(double**,double**,double**,int,int,int));
void write_matrix(char*, double**, int, int);
double **read_matrix(char*, int*, int*);
double pt     (double **c, double **a, double **b, int m, int p, int n); 
double omp    (double **c, double **a, double **b, int m, int p, int n);
double regular(double **c, double **a, double **b, int m, int p, int n);
void zeros(double **, int, int);
void free_matrix(double **, int );
void print_matrix(double **, int, int);
void transpose(double **, double **, int , int );



#endif
