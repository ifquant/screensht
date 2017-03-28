#include "window_info.h"

#include <stdio.h>

#include "display_info.h"
#include "args.h"

void window_info_init()
{
	window_info.size.x = XDisplayWidth(display_info.display, 0);
	window_info.size.y = XDisplayHeight(display_info.display, 0);

	XVisualInfo vinfo;
	XMatchVisualInfo(display_info.display, 0, 32, TrueColor, &vinfo);

	XSetWindowAttributes attr;
	attr.override_redirect = 1;
	attr.background_pixel = 0;
	attr.event_mask = StructureNotifyMask | ExposureMask;
	attr.colormap = XCreateColormap(display_info.display, display_info.root, vinfo.visual, AllocNone);

	window_info.window = XCreateWindow(
			display_info.display,
			XRootWindow(display_info.display, 0),
			0,
			0,
			window_info.size.x,
			window_info.size.y,
			0,
			vinfo.depth,
			InputOutput,
			vinfo.visual,
			CWBackPixel | CWColormap | CWBorderPixel | CWOverrideRedirect,
			&attr
	);

	XMapWindow(display_info.display, window_info.window);

	XGCValues gcv;
	gcv.foreground = args.color;
	gcv.function = GXcopy;
	gcv.subwindow_mode = IncludeInferiors;

	window_info.gc = XCreateGC(
			display_info.display,
			window_info.window,
			GCFunction | GCForeground | GCBackground | GCSubwindowMode,
			&gcv
	);

	XGCValues gcv_secondary;
	gcv_secondary.foreground = args.color_secondary;
	gcv_secondary.function = GXcopy;
	gcv_secondary.subwindow_mode = IncludeInferiors;

	window_info.gc_secondary = XCreateGC(
			display_info.display,
			window_info.window,
			GCFunction | GCForeground | GCBackground | GCSubwindowMode,
			&gcv_secondary
	);

	window_info.font = XLoadQueryFont(display_info.display, args.fontname);

	if (!window_info.font)
	{
		printf("failed loading font\n");
	}

	XSetFont(display_info.display, window_info.gc, window_info.font->fid);
	XSetFont(display_info.display, window_info.gc_secondary, window_info.font->fid);
}

void window_info_kill(window_info_t* info)
{
	XFreeFont(display_info.display, info->font);
	XFreeGC(display_info.display, info->gc);
	XFreeGC(display_info.display, info->gc_secondary);
	XDestroyWindow(display_info.display, info->window);
}
