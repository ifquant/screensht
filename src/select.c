#include "select.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include "display_info.h"
#include "window_info.h"
#include "args.h"

void draw_rectangle(area_t area)
{
	const int width = 4;
	for (int i = 1; i < width; i++)
	{
		GC gc = i == 1 || i == width - 1 ? window_info.gc_secondary : window_info.gc;
		XDrawRectangle(
				display_info.display,
				window_info.window,
				gc,
				area.x - i,
				area.y - i,
				area.w + i * 2 - 1,
				area.h + i * 2 - 1
		);
	}
}

void draw_text(vec2_t pos, char* text)
{
	// outline
	int sides[][2] = { { -1, -1 }, { 1, -1 }, { -1, 1 }, { 1, 1 }, { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } };
	for (int i = 0; i < sizeof(sides) / sizeof(int) / 2; i++)
	{
		XDrawString(
				display_info.display,
				window_info.window,
				window_info.gc_secondary,
				pos.x + sides[i][0],
				pos.y + sides[i][1],
				text,
				strlen(text)
		);
	}

	XDrawString(
			display_info.display,
			window_info.window,
			window_info.gc,
			pos.x,
			pos.y,
			text,
			strlen(text)
	);
}

char* get_size_string(area_t area)
{
	char buffer[32];
	sprintf(buffer, "%dx%d", area.w, area.h);
	return strdup(buffer);
}

void draw_selection(area_t area)
{
	char* text = get_size_string(area);

	int direction;
	int ascent;
	int descent;
	XCharStruct overall;
	XTextExtents(window_info.font, text, (int)strlen(text), &direction, &ascent, &descent, &overall);
	vec2_t text_pos = { area.x + area.w + 7, area.y + ascent };

	XClearWindow(display_info.display, window_info.window);
	draw_rectangle(area);
	draw_text(text_pos, text);

	free(text);
}

int grab_input(Cursor cursor)
{
	int status_pointer = XGrabPointer(
			display_info.display,
			display_info.root,
			0,
			ButtonMotionMask | ButtonPressMask | ButtonReleaseMask,
			GrabModeAsync,
			GrabModeAsync,
			display_info.root,
			cursor,
			CurrentTime
	);

	XGrabKeyboard(
			display_info.display,
			display_info.root,
			0,
			GrabModeAsync,
			GrabModeAsync,
			CurrentTime
	);

	if (status_pointer == AlreadyGrabbed)
	{
		return 1;
	}

	return 0;
}

void ungrab_input()
{
	XUngrabPointer(display_info.display, CurrentTime);
	XUngrabKeyboard(display_info.display, CurrentTime);
}

area_t select_area()
{
	Cursor cursor = XCreateFontCursor(display_info.display, XC_tcross);

	if (grab_input(cursor))
	{
		printf("cursor is already grabbed\n");
		XFreeCursor(display_info.display, cursor);
		return area_null();
	}

	vec2_t p1 = { 0, 0 };
	int pressed = 0;
	XEvent event;

	while (!pressed)
	{
		XNextEvent(display_info.display, &event);

		if (event.type == ButtonPress && event.xbutton.button == Button1)
		{
			pressed = 1;
			p1.x = event.xbutton.x;
			p1.y = event.xbutton.y;
		}
		else if ((event.type == ButtonPress && event.xbutton.button != Button1)
		      || (event.type == KeyPress))
		{
			ungrab_input();
			XFreeCursor(display_info.display, cursor);
			return area_null();
		}
	}

	vec2_t p2 = p1;
	int up = 0;

	while (!up)
	{
		XNextEvent(display_info.display, &event);

		if (event.type == MotionNotify)
		{
			p2.x = event.xmotion.x;
			p2.y = event.xmotion.y;
			draw_selection(area_positions_to_area(p1, p2));
		}
		else if (event.type == ButtonRelease && event.xbutton.button == Button1)
		{
			up = 1;
		}
		else if ((event.type == ButtonPress && event.xbutton.button != Button1)
		      || (event.type == KeyPress))
		{
			ungrab_input();
			XFreeCursor(display_info.display, cursor);
			return area_null();
		}

	}

	ungrab_input();
	XFreeCursor(display_info.display, cursor);
	XClearWindow(display_info.display, window_info.window);

	return area_positions_to_area(p1, p2);
}
