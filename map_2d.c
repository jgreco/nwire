#include <stdlib.h>
#include <string.h>
#include "map_2d.h"

map_2d map_create(int x, int y)
{
	int i;
	map_2d map;


	map = (map_2d) malloc(sizeof(map_2d));

	map->data = (MAPTYPE** ) malloc(y*sizeof(MAPTYPE* ));

	for(i=0; i<y; i++)
		map->data[i] = (MAPTYPE* ) malloc(x*sizeof(MAPTYPE));

	map->width = x;
	map->height = y;

	return map;
}

map_2d map_copy(map_2d map)
{
	int i;

	map_2d new;
	new = map_create(map->width, map->height);

	for(i=0; i<map->height; i++)
		memcpy(new->data[i], map->data[i], sizeof(MAPTYPE)*map->width);

	return new;
}

void map_destroy(map_2d map)
{
	free(map->data);
	free(map);

	return;
}

MAPTYPE map_write(map_2d map, int x, int y, MAPTYPE data)
{
	map->data[y][x] = data;

	return 0;
}

MAPTYPE map_access(map_2d map, int x, int y)
{
	return map->data[y][x];
}
