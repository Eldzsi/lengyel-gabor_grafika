#include <stdio.h>
#include "matrix.h"


void init_identity_matrix(Matrix* m) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i == j) {
                m->data[i][j] = 1.0;
            } else {
                m->data[i][j] = 0.0;
            }
        }
    }
}


void multiply_scalar(Matrix* m, double scalar) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            m->data[i][j] *= scalar;
        }
    }
}


void multiply_matrices(const Matrix* m1, const Matrix* m2, Matrix* result) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result->data[i][j] = 0.0;
            for (int k = 0; k < SIZE; k++) {
                result->data[i][j] += m1->data[i][k] * m2->data[k][j];
            }
        }
    }
}


void print_matrix(const Matrix* m) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%.2f ", m->data[i][j]);
        }
        printf("\n");
    }
}


void print_point(const Point* p) {
    printf("Point: (%.2f, %.2f, %.2f)\n", p->x, p->y, p->z);
}