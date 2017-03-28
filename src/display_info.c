#include "display_info.h"

void display_info_init()
{
	display_info.display = XOpenDisplay(0);
	display_info.root = DefaultRootWindow(display_info.display);
	display_info.screen = ScreenOfDisplay(display_info.display, 0); // screen->width, screen->height
}

void display_info_kill(display_info_t* info)
{
	XDestroyWindow(info->display, info->root);
	XCloseDisplay(info->display);
}
