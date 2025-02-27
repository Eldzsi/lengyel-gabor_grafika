#include "cuboid.h"


void set_cuboid_size(Cuboid* cuboid, double x, double y, double z) {
	cuboid->x = x;
	cuboid->y = y;
    cuboid->z = z;
}


int has_cuboid_square(const Cuboid* cuboid) {
    return (cuboid->x == cuboid->y) || (cuboid->x == cuboid->z) || (cuboid->y == cuboid->z);
}


double calc_cuboid_volume(const Cuboid* cuboid) {
    return cuboid->x * cuboid->y * cuboid->z;
}


double calc_cuboid_surface(const Cuboid* cuboid) {
    return 2 * (cuboid->x * cuboid->y + cuboid->x * cuboid->z + cuboid->y * cuboid->z);
}