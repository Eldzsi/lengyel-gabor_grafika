#include <stdio.h>
#include "cuboid.h"


int main() {
    Cuboid cuboid;
    double x, y, z;

    printf("\nAdja meg a teglatest 3 elenek hosszat: ");
    if (scanf("%lf %lf %lf", &x, &y, &z) != 3) {
        printf("\nHibas input!");
        return 1;
    }

    set_cuboid_size(&cuboid, x, y, z);

    double volume = calc_cuboid_volume(&cuboid);
    double surface = calc_cuboid_surface(&cuboid);

    printf("\nTeglatest terfogata: %.2lf", volume);
    printf("\nTeglatest felszine: %.2lf", surface);

    if (has_cuboid_square(&cuboid)) {
        printf("\nA teglatestnek van negyzet alaku lapja.");
    } else {
        printf("\nA teglatestnek nincs negyzet alaku lapja.");
    }

    return 0;
}