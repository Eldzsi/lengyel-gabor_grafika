#include "utils.h"

#include <obj/model.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


double degree_to_radian(double degree) {
	return degree * M_PI / 180.0;
}


void rotate_point(float* x, float* y, float* z, float rx, float ry, float rz) {
    float rad_x = degree_to_radian(rx);
    float rad_y = degree_to_radian(ry);
    float rad_z = degree_to_radian(rz);

    float new_y = *y * cos(rad_x) - *z * sin(rad_x);
    float new_z = *y * sin(rad_x) + *z * cos(rad_x);
    *y = new_y;
    *z = new_z;

    float new_x = *x * cos(rad_y) + *z * sin(rad_y);
    *z = -(*x * sin(rad_y)) + *z * cos(rad_y);
    *x = new_x;

    new_x = *x * cos(rad_z) - *y * sin(rad_z);
    new_y = *x * sin(rad_z) + *y * cos(rad_z);
    *x = new_x;
    *y = new_y;
}


void get_model_size(const Model* model, float* width, float* depth, float* height, vec3* min, vec3* max) {
    if (model->n_vertices == 0 || model->vertices == NULL) {
        *width = *depth = *height = 0.0f;
        return;
    }

    double min_x = model->vertices[0].x, max_x = model->vertices[0].x;
    double min_y = model->vertices[0].y, max_y = model->vertices[0].y;
    double min_z = model->vertices[0].z, max_z = model->vertices[0].z;

    for (int i = 1; i < model->n_vertices; i++) {
        if (model->vertices[i].x < min_x) min_x = model->vertices[i].x;
        if (model->vertices[i].x > max_x) max_x = model->vertices[i].x;

        if (model->vertices[i].y < min_y) min_y = model->vertices[i].y;
        if (model->vertices[i].y > max_y) max_y = model->vertices[i].y;

        if (model->vertices[i].z < min_z) min_z = model->vertices[i].z;
        if (model->vertices[i].z > max_z) max_z = model->vertices[i].z;
    }

    *width = (float)(max_x - min_x);
    *depth = (float)(max_y - min_y);
    *height = (float)(max_z - min_z);

    min->x = min_x;
    min->y = min_y;
    min->z = min_z;

    max->x = max_x;
    max->y = max_y;
    max->z = max_z;
}


double generate_random_number(double min, double max) {
    return ((double)rand() / RAND_MAX) * (max - min) + min;
}