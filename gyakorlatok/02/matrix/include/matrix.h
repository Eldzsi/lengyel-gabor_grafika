#ifndef MATRIX_H
#define MATRIX_H

#define SIZE 3


typedef struct Matrix {
    double data[SIZE][SIZE];
} Matrix;


typedef struct Point {
    double x, y, z;
} Point;


void init_identity_matrix(Matrix* m);

void multiply_scalar(Matrix* m, double scalar);

void multiply_matrices(const Matrix* m1, const Matrix* m2, Matrix* result);

void print_matrix(const Matrix* m);

void print_point(const Point* p);

#endif
