#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>
#define USER 943
#define MOVIE 1682
#define GENRE 19
#define USER_COLUMN 21
#define MAX_RATING 5
#define TOP 10
#define NO_OF_USER 50
#define MAX 1000
void func_main(int **,int **,int,int,int,int,int);
double* distance(int **,int **,int,int,int,int);
int* kmin(double *,int,int);
int majority(int *,int);
