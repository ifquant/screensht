#include "args.h"

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "display_info.h"
#include "hosts.h"
#include "save.h"

arg_values_t arg_values;

void end()
{
	display_info_kill(&display_info);
	exit(0);
}

const char* random_str = "random";

void parse_host(void* out, char* in)
{
	if (!strcmp(in, random_str))
	{
		*(host_t*)out = hosts[rand() % n_hosts];
		return;
	}

	for (int i = 0; i < n_hosts; i++)
	{
		if (!strcmp(hosts[i].arg_name, in))
		{
			*(host_t*)out = hosts[i];
			return;
		}
	}

	printf("invalid host name\n");
	end();
}

void parse_int(void* out, char* in)
{
	*(int*)out = atoi(in);
}

void parse_font(void* out, char* in)
{
	if (!strcmp(in, random_str))
	{
		int fonts_n;
		char** fonts = XListFonts(display_info.display, "*", INT_MAX, &fonts_n);
		*(char**)out = fonts[rand() % fonts_n];
		return;
	}

	*(char**)out = in;
}

void parse_color(void* out, char* in)
{
	if (!strcmp(in, random_str))
	{
		unsigned long result = 0xff000000; // so alpha is always 100%
		result += rand() % 0xffffff;
		*(unsigned long*)out = result;
		return;
	}

	*(unsigned long*)out = (unsigned long)strtol(in, 0, 16);
}

void parse_charptr(void* out, char* in)
{
	*(char**)out = in;
}

void parse_form_filename(void* out, char* in)
{
	if (!strcmp(in, random_str))
	{
		char* fname = rand_string(16);
		char* ext = rand_string(6);
		asprintf((char**)out, "%s.%s", fname, ext);
		free(ext);
		free(fname);
		return;
	}

	*(char**)out = in;
}

arg_t args[] = {
		{ 'h', "--host", &arg_values.host, &parse_host },
		{ 'q', "--quality", &arg_values.quality, &parse_int },
		{ 'f', "--font", &arg_values.font, &parse_font },
		{ 'c', "--color", &arg_values.color, &parse_color },
		{ 's', "--secondary-color", &arg_values.secondary_color, &parse_color },
		{ 'k', "--keep", &arg_values.keep, &parse_charptr },
		{ 'n', "--form-filename", &arg_values.form_filename, &parse_form_filename } 
};
int n_args = sizeof(args) / sizeof(args[0]);

void help()
{
	printf("options:\n");

	for (int i = 0; i < n_args; i++)
	{
		printf("\t-%c\t%s\n", args[i].char_option, args[i].full_option);
	}

	printf("\nhttps://github.com/nvllvs/screensht#options\n");
	end();
}

int test_arg(char* arg_provided, char* next_arg)
{
	for (int i = 0; i < n_args; i++)
	{
		if (strcmp(arg_provided, args[i].full_option) && arg_provided[1] != args[i].char_option)
		{
			if (i + 1 == n_args)
			{
				help();
				return 0;
			}

			continue;
		}

		args[i].parse_func(args[i].ptr, next_arg);

		return 1;
	}

	return 0;
}

void args_init(int argc, char** argv)
{
	arg_values.host = hosts[0];
	arg_values.quality = 99;
	arg_values.font = "*x14";
	arg_values.color = 0xfff0f0f0;
	arg_values.secondary_color = 0xff000000;
	arg_values.keep = "";
	arg_values.form_filename = "screensht.jpg";

	if (argc <= 1)
	{
		return;
	}

	for (int i = 1; i < argc; i++)
	{
		i += test_arg(argv[i], argv[i + 1]);
	}
}
