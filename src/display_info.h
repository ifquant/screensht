#ifndef DISPLAY_INFO_H
#define DISPLAY_INFO_H

#include <X11/Xutil.h>

typedef struct
{
	Display* display;
	Window root;
	Screen* screen;
} display_info_t;

void display_info_init();
void display_info_kill(display_info_t* info);

display_info_t display_info;

#endif
