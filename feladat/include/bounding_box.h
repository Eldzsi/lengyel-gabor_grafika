#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H


#include "utils.h"


typedef struct Scene Scene;

typedef struct Object Object;

typedef struct BoundingBox {
    vec3 min;
    vec3 max;
} BoundingBox;


/*
Calculate the bounding box of an object, taking into account its scale, rotation, and position.
A z_offset can be applied to adjust the height of the bounding box.
*/
BoundingBox calculate_bounding_box(const Object* obj, float z_offset);

/*
Update all bounding boxes for the objects present in the scene.
*/
void update_bounding_boxes(Scene* scene);

/*
Draw the bounding boxes of all objects in the scene for debugging purposes.
*/
void draw_bounding_boxes(const Scene* scene);


#endif
