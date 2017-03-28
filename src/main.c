#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#include "area.h"
#include "display_info.h"
#include "window_info.h"
#include "args.h"
#include "select.h"
#include "capture.h"
#include "upload.h"

void notify(char* url, float time)
{
	printf("%s\nupload took %3.f ms\n", url, time);

	char buffer[256];
	sprintf(buffer, "notify-send \"%s\" \"upload took %3.f ms\"", url, time);
	system(buffer);
}

void copy_to_clipboard(char* text)
{
	char buffer[256];
	sprintf(buffer, "echo \"%s\" | xsel -i -b", text);
	system(buffer);
}

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
			args.directory,
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

float get_elapsed(struct timeval start)
{
	struct timeval end;
	gettimeofday(&end, 0);
	float elapsed = (end.tv_sec - start.tv_sec) * 1000.f;
	elapsed += (end.tv_usec - start.tv_usec) / 1000.f;
	return elapsed;
}

void mkpath(char* path)
{
	char* p;

	for (p = strchr(path + 1, '/'); p; p = strchr(p + 1, '/'))
	{
		*p = '\0';
		mkdir(path, 0755);
		*p = '/';
	}
}

void check_path()
{
	struct stat st;
	if (stat(args.directory, &st) == -1)
	{
		printf("creating directory path %s\n", args.directory);
		mkpath(args.directory);
	}
	else
	{
		printf("directory path %s found\n", args.directory);
	}
}

int main(int argc, char** argv)
{
	srand((unsigned int)time(0));

	display_info_init();
	args_init(argc, argv);
	window_info_init();

	check_path();

	char* filename = get_filename();
	area_t area = select_area();

	if (area_is_null(area))
	{
		printf("area is null\n");
		window_info_kill(&window_info);
		display_info_kill(&display_info);
		free(filename);
		return 0;
	}

	window_info_kill(&window_info);

	struct timeval start;
	gettimeofday(&start, 0);

	capture_sht(area, filename);

	display_info_kill(&display_info);

	char* url = upload_sht(filename);

	free(filename);

	float elapsed = get_elapsed(start);
	notify(url, elapsed);
	copy_to_clipboard(url);

	free(url);

	return 0;
}
