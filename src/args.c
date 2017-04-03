#include "args.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "display_info.h"
#include "hosts.h"
#include "lock.h"

args_t args;

void args_init(int argc, char** argv)
{
	// defaults
	args.host = hosts[0];
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
		if (!strcmp(argv[i], "-h"))
		{
			i++;
			int hosts_n = n_hosts;
			if (!strcmp(argv[i], "random"))
			{
				args.host = hosts[rand() % hosts_n];
			}
			else
			{
				for (int j = 0; j < hosts_n; j++)
				{
					if (!strcmp(argv[i], hosts[j].arg_name))
					{
						args.host = hosts[j];
						break;
					}
				}
			}
		}
		else if (!strcmp(argv[i], "-q"))
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
			args.color = (unsigned long)strtol(argv[i], 0, 16);
		}
		else if (!strcmp(argv[i], "-s"))
		{
			i++;
			args.color_secondary = (unsigned long)strtol(argv[i], 0, 16);
		}
		else
		{
			printf(
					"options:\n"
					"\t-h\thost\n"
					"\t-q\tquality, 0-100\n"
					"\t-d\tdirectory\n"
					"\t-f\tfont name\n"
					"\t-c\tcolor in ARGB hex code\n"
					"\t-s\tsecondary color\n"
			);
			display_info_kill(&display_info);
			lock_kill();
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
