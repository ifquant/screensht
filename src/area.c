#include "area.h"

int area_is_null(area_t area)
{
	return area.x == 0 && area.y == 0 && area.w == 0 && area.h == 0;
}

area_t area_null()
{
	area_t area_null = { 0, 0, 0, 0 };
	return area_null;
}

area_t area_positions_to_area(vec2_t p1, vec2_t p2)
{
	if (vec2_are_equal(p1, p2))
	{
		return area_null();
	}

	int x[] = { p1.x, p2.x };
	int y[] = { p1.y, p2.y };
	int i_x = (int)(x[1] > x[0]);
	int i_y = (int)(y[1] > y[0]);
	area_t area = { x[!i_x], y[!i_y], x[i_x] - x[!i_x], y[i_y] - y[!i_y] };
	return area;
}
