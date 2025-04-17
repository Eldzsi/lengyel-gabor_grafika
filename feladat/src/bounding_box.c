#include "bounding_box.h"
#include "scene.h"
#include "utils.h"

#include "math.h"


BoundingBox calculate_bounding_box(const Object* obj, float z_offset) {
    BoundingBox box;

    float min_x = obj->min.x * obj->scale.x;
    float max_x = obj->max.x * obj->scale.x;
    float min_y = obj->min.y * obj->scale.y;
    float max_y = obj->max.y * obj->scale.y;
    float min_z = obj->min.z * obj->scale.z;
    float max_z = obj->max.z * obj->scale.z;

    float corners[8][3] = {
        {min_x, min_y, min_z}, {max_x, min_y, min_z}, {max_x, max_y, min_z}, {min_x, max_y, min_z},
        {min_x, min_y, max_z}, {max_x, min_y, max_z}, {max_x, max_y, max_z}, {min_x, max_y, max_z}
    };

    float rad_x = degree_to_radian(obj->rotation.x);
    float rad_y = degree_to_radian(obj->rotation.y);
    float rad_z = degree_to_radian(obj->rotation.z);

    float Rx[3][3] = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, cos(rad_x), -sin(rad_x)},
        {0.0f, sin(rad_x), cos(rad_x)}
    };
    
    float Ry[3][3] = {
        {cos(rad_y), 0.0f, sin(rad_y)},
        {0.0f, 1.0f, 0.0f},
        {-sin(rad_y), 0.0f, cos(rad_y)}
    };

    float Rz[3][3] = {
        {cos(rad_z), -sin(rad_z), 0.0f},
        {sin(rad_z), cos(rad_z), 0.0f},
        {0.0f, 0.0f, 1.0f}
    };

    for (int i = 0; i < 8; i++) {
        float x = corners[i][0];
        float y = corners[i][1];
        float z = corners[i][2];

        float x_rot = Rz[0][0] * x + Rz[0][1] * y + Rz[0][2] * z;
        float y_rot = Rz[1][0] * x + Rz[1][1] * y + Rz[1][2] * z;
        float z_rot = Rz[2][0] * x + Rz[2][1] * y + Rz[2][2] * z;

        x = Ry[0][0] * x_rot + Ry[0][1] * y_rot + Ry[0][2] * z_rot;
        y = Ry[1][0] * x_rot + Ry[1][1] * y_rot + Ry[1][2] * z_rot;
        z = Ry[2][0] * x_rot + Ry[2][1] * y_rot + Ry[2][2] * z_rot;

        x_rot = Rx[0][0] * x + Rx[0][1] * y + Rx[0][2] * z;
        y_rot = Rx[1][0] * x + Rx[1][1] * y + Rx[1][2] * z;
        z_rot = Rx[2][0] * x + Rx[2][1] * y + Rx[2][2] * z;

        corners[i][0] = x_rot;
        corners[i][1] = y_rot;
        corners[i][2] = z_rot;
    }

    box.min.x = box.max.x = corners[0][0];
    box.min.y = box.max.y = corners[0][1];
    box.min.z = box.max.z = corners[0][2];

    for (int i = 1; i < 8; i++) {
        if (corners[i][0] < box.min.x) box.min.x = corners[i][0];
        if (corners[i][0] > box.max.x) box.max.x = corners[i][0];

        if (corners[i][1] < box.min.y) box.min.y = corners[i][1];
        if (corners[i][1] > box.max.y) box.max.y = corners[i][1];

        if (corners[i][2] < box.min.z) box.min.z = corners[i][2];
        if (corners[i][2] > box.max.z) box.max.z = corners[i][2];
    }

    float padding_x = 0.12f;
    float padding_y = 0.12f;
    float padding_z = 0.12f;

    box.min.x -= padding_x;
    box.max.x += padding_x;
    box.min.y -= padding_y;
    box.max.y += padding_y;
    box.min.z -= padding_z;
    box.max.z += padding_z;

    box.min.x += obj->position.x;
    box.min.y += obj->position.y;
    box.min.z += obj->position.z;

    box.max.x += obj->position.x;
    box.max.y += obj->position.y;
    box.max.z += obj->position.z + z_offset;

    return box;
}


void draw_bounding_boxes(const Scene* scene) {
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 0.0f);

    for (int i = 0; i < scene->object_count; i++) {
        BoundingBox box = scene->bounding_boxes[i];

        glBegin(GL_LINES);
            glVertex3f(box.min.x, box.min.y, box.min.z); glVertex3f(box.max.x, box.min.y, box.min.z);
            glVertex3f(box.max.x, box.min.y, box.min.z); glVertex3f(box.max.x, box.max.y, box.min.z);
            glVertex3f(box.max.x, box.max.y, box.min.z); glVertex3f(box.min.x, box.max.y, box.min.z);
            glVertex3f(box.min.x, box.max.y, box.min.z); glVertex3f(box.min.x, box.min.y, box.min.z);

            glVertex3f(box.min.x, box.min.y, box.max.z); glVertex3f(box.max.x, box.min.y, box.max.z);
            glVertex3f(box.max.x, box.min.y, box.max.z); glVertex3f(box.max.x, box.max.y, box.max.z);
            glVertex3f(box.max.x, box.max.y, box.max.z); glVertex3f(box.min.x, box.max.y, box.max.z);
            glVertex3f(box.min.x, box.max.y, box.max.z); glVertex3f(box.min.x, box.min.y, box.max.z);

            glVertex3f(box.min.x, box.min.y, box.min.z); glVertex3f(box.min.x, box.min.y, box.max.z);
            glVertex3f(box.max.x, box.min.y, box.min.z); glVertex3f(box.max.x, box.min.y, box.max.z);
            glVertex3f(box.max.x, box.max.y, box.min.z); glVertex3f(box.max.x, box.max.y, box.max.z);
            glVertex3f(box.min.x, box.max.y, box.min.z); glVertex3f(box.min.x, box.max.y, box.max.z);
        glEnd();
    }

    glEnable(GL_LIGHTING);
}
