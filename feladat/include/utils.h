#ifndef UTILS_H
#define UTILS_H


#include <obj/model.h>


typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

typedef struct Color {
    float red;
    float green;
    float blue;
} Color;

typedef struct Material {
    struct Color ambient;
    struct Color diffuse;
    struct Color specular;
    float shininess;
} Material;


double degree_to_radian(double degree);

void rotate_point(float* x, float* y, float* z, float rx, float ry, float rz);

void get_model_size(const Model* model, float* width, float* depth, float* height, vec3* min, vec3* max);

double generate_random_number(double min, double max);


#endif
