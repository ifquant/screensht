#ifndef ARGS_H
#define ARGS_H

#include "hosts.h"

typedef struct
{
	char char_option;
	char* full_option;
	void* ptr;
	void (*parse_func)(void*, char*);
} arg_t;

typedef struct
{
	host_t host;
	int quality;
	char* font;
	unsigned long color;
	unsigned long secondary_color;
	char* keep;
	char* form_filename;
	int verbose;
} arg_values_t;

void args_init(int argc, char** argv);

extern arg_t args[];
extern arg_values_t arg_values;

#endif
