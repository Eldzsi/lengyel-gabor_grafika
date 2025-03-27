#include <stdio.h>
#include "matrix.h"


int main() {
    Matrix m1, m2, result;
    init_identity_matrix(&m1);
    init_identity_matrix(&m2);

    printf("Identity Matrix:\n");
    print_matrix(&m1);

    printf("\nMultiplying identity matrix by scalar 2.0:\n");
    multiply_scalar(&m1, 2.0);
    print_matrix(&m1);

    printf("\nMultiplying two identity matrices:\n");
    multiply_matrices(&m1, &m2, &result);
    print_matrix(&result);

    Point p = {1.0, 2.0, 1.0};
    printf("\nInitial Point:\n");
    print_point(&p);

    return 0;
}
