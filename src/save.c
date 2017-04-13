#include "save.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include "args.h"

char* rand_string(int n)
{
	char chars[] = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";
	char* result = malloc(sizeof(char) * (n - 1));

	for (int i = 0; i < n; i++)
	{
		result[i] = chars[rand() % (sizeof(chars) - 1)];
	}

	result[n] = 0;

	return result;
}

char* get_filename()
{
	time_t t = time(0);
	struct tm tm = *localtime(&t);
	char buffer[128];
	char* rand = rand_string(4);
	sprintf(
			buffer,
			"%sscreensht_%04d-%02d-%02d_%02d.%02d.%02d_%s.jpg",
			arg_values.keep,
			1900 + tm.tm_year,
			1 + tm.tm_mon,
			tm.tm_mday,
			tm.tm_hour,
			tm.tm_min,
			tm.tm_sec,
			rand
	);
	free(rand);
	return strdup(buffer);
}

void mkpath(char* path)
{
	for (char* p = strchr(path + 1, '/'); p; p = strchr(p + 1, '/'))
	{
		*p = '\0';
		mkdir(path, 0755);
		*p = '/';
	}
}

void check_path(char* path)
{
	struct stat st;

	if (stat(path, &st) == -1)
	{
		printf("creating directory path %s\n", path);
		mkpath(path);
	}
	else
	{
		printf("directory path %s found\n", path);
	}
}

void save_to_file(char* filename, unsigned char* buffer, unsigned long size)
{
	FILE* file = fopen(filename, "w");

	if (!file)
	{
		printf("error opening file\n");
	}

	fwrite((char*)buffer, sizeof(char), size, file);
	fclose(file);
}

void save(unsigned char* buffer, unsigned long size)
{
	char* filename = get_filename();
	check_path(filename);
	save_to_file(filename, buffer, size);
	free(filename);
}
