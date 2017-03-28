#ifndef WINDOW_INFO_H
#define WINDOW_INFO_H

#include <X11/Xutil.h>
#include "vec2.h"

typedef struct
{
	Window window;
	vec2_t size;
	GC gc;
	GC gc_secondary;
	XFontStruct* font;
} window_info_t;

void window_info_init();
void window_info_kill(window_info_t* info);

window_info_t window_info;

#endif
