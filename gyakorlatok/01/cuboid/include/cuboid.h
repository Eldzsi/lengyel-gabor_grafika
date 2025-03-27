#ifndef CUBOID_H
#define CUBOID_H


typedef struct Cuboid {
	double x, y, z;
} Cuboid;


void set_cuboid_size(Cuboid* cuboid, double x, double y, double z);

int has_cuboid_square(const Cuboid* cuboid);

double calc_cuboid_volume(const Cuboid* cuboid);

double calc_cuboid_surface(const Cuboid* cuboid);

#endif
