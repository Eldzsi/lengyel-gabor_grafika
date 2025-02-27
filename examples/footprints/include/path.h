#ifndef PATH_H
#define PATH_H

typedef struct Vec2 {
  double x;
  double y;
} Vec2;

typedef struct Footprint {
  Vec2 position;
  struct Footprint* next;
} Footprint;

typedef struct Path {
  Footprint* footprints;
} Path;

/**
 * Create a new path.
 */
void init_path(Path* path);

/**
 * Add a new footprint to the path.
 */
void add_footprint_to_path(Path* path, double x, double y);

/**
 * Render the path.
 */
void render_path(const Path* path);

/**
 * Draw a single footprint.
 */
void draw_footprint(const Footprint* footprint);

/**
 * Destroy the path.
 */
void destroy_path(Path* path);

#endif /* PATH_H */

