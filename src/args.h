#ifndef ARGS_H
#define ARGS_H

#include "hosts.h"

typedef struct
{
	host_t host;
	int quality;
	char* directory;
	char* fontname;
	unsigned long color;
	unsigned long color_secondary;
} args_t;

void args_init(int argc, char** argv);

extern args_t args;

#endif
