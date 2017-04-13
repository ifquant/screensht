#ifndef ARGS_H
#define ARGS_H

#include "hosts.h"

enum
{
	TYPE_HOST_T = 0,
	TYPE_INT,
	TYPE_CHARPTR,
	TYPE_ULONG
};

typedef struct
{
	char* short_name;
	char* name;
	void* ptr;
	int type;
} arg_t;

typedef struct
{
	host_t host;
	int quality;
	char* font;
	unsigned long color;
	unsigned long secondary_color;
	char* keep;
} arg_values_t;

void args_init(int argc, char** argv);

extern arg_t args[];
extern arg_values_t arg_values;

#endif
