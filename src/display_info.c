#include "display_info.h"

#include <stdio.h>

int has_composite_manager()
{
	Atom cm_atom = XInternAtom(display_info.display, "_NET_WM_CM_S0", 0);
	Window cm_owner = XGetSelectionOwner(display_info.display, cm_atom);
	return !!cm_owner;
}

void display_info_init()
{
	display_info.display = XOpenDisplay(0);
	display_info.root = DefaultRootWindow(display_info.display);
	display_info.screen = ScreenOfDisplay(display_info.display, 0); // screen->width, screen->height
	display_info.has_composite_manager = has_composite_manager();

	if (!display_info.has_composite_manager)
	{
		printf(
				"running screensht in compatibility mode (no composite manager).\n"
				"some features such as colors are disabled.\n"
				"for more info have a look at:\n"
				"https://github.com/nvllvs/screensht#compatibility-mode\n"
		);
	}
}

void display_info_kill(display_info_t* info)
{
	XDestroyWindow(info->display, info->root);
	XCloseDisplay(info->display);
}
