#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H


#include "utils.h"


typedef struct Scene Scene;

typedef struct Object Object;

typedef struct BoundingBox {
    vec3 min;
    vec3 max;
} BoundingBox;


BoundingBox calculate_bounding_box(const Object* obj, float z_offset);

void update_bounding_boxes(Scene* scene);

void draw_bounding_boxes(const Scene* scene);


#endif
