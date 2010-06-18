#ifndef MAP_2D_H_
#define MAP_2D_H_

#include <stdint.h>

#define MAPTYPE uint8_t


typedef struct map_2d_str {
	MAPTYPE **data;

	int width;
	int height;
} *map_2d;

map_2d map_create(int x, int y);
map_2d map_copy(map_2d map);
void map_destroy(map_2d map);
MAPTYPE map_write(map_2d map, int x, int y, MAPTYPE data);
MAPTYPE map_access(map_2d map, int x, int y);

#endif
