#ifndef ARGS_H
#define ARGS_H

typedef struct
{
	int quality;
	char* directory;
	char* fontname;
	unsigned long color;
	unsigned long color_secondary;
} args_t;

void args_init(int argc, char** argv);

args_t args;

#endif
