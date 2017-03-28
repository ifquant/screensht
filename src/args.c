#include "args.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "display_info.h"

void args_init(int argc, char** argv)
{
	// defaults
	args.quality = 99;
	args.directory = "./";
	args.fontname = "*x14";
	args.color = 0xfff0f0f0; // ARGB
	args.color_secondary = 0xff000000;

	if (argc <= 1)
	{
		return;
	}

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-q"))
		{
			i++;
			args.quality = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-d"))
		{
			i++;
			args.directory = argv[i];
		}
		else if (!strcmp(argv[i], "-f"))
		{
			i++;
			args.fontname = argv[i];
		}
		else if (!strcmp(argv[i], "-c"))
		{
			i++;
			args.color = strtol(argv[i], 0, 16);
		}
		else if (!strcmp(argv[i], "-s"))
		{
			i++;
			args.color_secondary = strtol(argv[i], 0, 16);
		}
		else
		{
			printf(
					"options:\n"
					"\t-q\tquality, 0-100\n"
					"\t-d\tdirectory\n"
					"\t-f\tfont name\n"
					"\t-c\tcolor in ARGB hex code\n"
					"\t-s\tsecondary color\n"
			);
			display_info_kill(&display_info);
			exit(0);
		}
	}

	if (args.directory[strlen(args.directory) - 1] != '/')
	{
		// easier to work with
		strcat(args.directory, "/");
	}

	if (!strcmp(args.fontname, "random"))
	{
		int fonts_n;
		char** fonts = XListFonts(display_info.display, "*", INT_MAX, &fonts_n);
		args.fontname = fonts[rand() % fonts_n];
	}
}
