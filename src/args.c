#include "args.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "display_info.h"
#include "hosts.h"
#include "save.h"

arg_values_t arg_values;
arg_t args[] = {
		{ "-h", "--host", .ptr = &arg_values.host, .type = TYPE_HOST_T },
		{ "-q", "--quality", .ptr = &arg_values.quality, .type = TYPE_INT },
		{ "-f", "--font", .ptr = &arg_values.font, .type = TYPE_CHARPTR },
		{ "-c", "--color", .ptr = &arg_values.color, .type = TYPE_ULONG },
		{ "-s", "--secondary-color", .ptr = &arg_values.secondary_color, .type = TYPE_ULONG },
		{ "-k", "--keep", .ptr = &arg_values.keep, .type = TYPE_CHARPTR },
		{ "-n", "--form-filename", .ptr = &arg_values.form_filename, .type = TYPE_CHARPTR }
};
int n_args = sizeof(args) / sizeof(args[0]);

void end()
{
	display_info_kill(&display_info);
	exit(0);
}

void help()
{
	printf("options:\n");

	for (int i = 0; i < n_args; i++)
	{
		printf("\t%s\t%s\n", args[i].short_name, args[i].name);
	}

	printf("\nhttps://github.com/nvllvs/screensht#options\n");
	end();
}

host_t find_host(char* name)
{
	for (int i = 0; i < n_hosts; i++)
	{
		if (!strcmp(hosts[i].arg_name, name))
		{
			return hosts[i];
		}
	}

	printf("invalid host name\n");
	end();
	return hosts[0];
}

void change_arg_value(arg_t arg, char* new_value)
{
	if (arg.type == TYPE_CHARPTR)
	{
		*(char**)arg.ptr = new_value;
	}
	else if (arg.type == TYPE_HOST_T)
	{
		*(host_t*)arg.ptr = find_host(new_value);
	}
	else if (arg.type == TYPE_INT)
	{
		*(int*)arg.ptr = atoi(new_value);
	}
	else if (arg.type == TYPE_ULONG)
	{
		*(unsigned long*)arg.ptr = (unsigned long)strtol(new_value, 0, 16);
	}
}

void args_init(int argc, char** argv)
{
	// defaults
	arg_values.host = hosts[0];
	arg_values.quality = 99;
	arg_values.font = "*x14";
	arg_values.color = 0xfff0f0f0; // ARGB
	arg_values.secondary_color = 0xff000000;
	arg_values.keep = "";
	arg_values.form_filename = "screensht.jpg";

	if (argc <= 1)
	{
		return;
	}

	for (int i = 1; i < argc; i++)
	{
		for (int j = 0; j < n_args; j++)
		{
			if (strcmp(argv[i], args[j].short_name) && strcmp(argv[i], args[j].name))
			{
				if (j + 1 == n_args)
				{
					help();
				}

				continue;
			}

			i++;

			change_arg_value(args[j], argv[i]);

			break;
		}
	}

	if (!strcmp(arg_values.font, "random"))
	{
		int fonts_n;
		char** fonts = XListFonts(display_info.display, "*", INT_MAX, &fonts_n);
		arg_values.font = fonts[rand() % fonts_n];
	}

	if (!strcmp(arg_values.form_filename, "random"))
	{
		char* fname = rand_string(16);
		char* ext = rand_string(6);
		sprintf(arg_values.form_filename, "%s.%s", fname, ext);
	}
}
