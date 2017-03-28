#ifndef AREA_H
#define AREA_H

#include "vec2.h"

typedef struct
{
	int x;
	int y;
	int w;
	int h;
} area_t;

int area_is_null(area_t area);
area_t area_null();
area_t area_positions_to_area(vec2_t p1, vec2_t p2);

#endif
