#include "hosts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const host_t const hosts[] = {
		{ "0x0.st", "file", "https://0x0.st/", &regular_response },
		{ "mixtape.moe", "files[]", "https://mixtape.moe/upload.php", &get_json_url },
		{ "nya.is", "files[]", "https://nya.is/upload", &get_json_url },
		{ "p.fuwafuwa.moe", "files[]", "https://p.fuwafuwa.moe/upload.php", &get_json_url },
		{ "safe.moe", "files[]", "https://safe.moe/api/upload", &get_json_url },
		{ "cocaine.ninja", "files[]", "https://cocaine.ninja/upload.php?output=text", &regular_response },
		{ "comfy.moe", "files[]", "https://comfy.moe/upload.php", &get_json_url },
		{ "pomf.cat", "files[]", "https://pomf.cat/upload.php", &pomf_cat },
		{ "lewd.se", "file", "https://lewd.se/api.php?d=upload-tool", &regular_response },
		{ "memenet.org", "files[]", "https://memenet.org/upload.php", &get_json_url },
		{ "uguu.se", "file", "https://uguu.se/api.php?d=upload-tool", &regular_response },
		{ "yiff.moe", "files[]", "https://yiff.moe/upload.php", &get_json_url },
		{ "vidga.me", "files[]", "https://vidga.me/upload.php", &get_json_url }
};

const int n_hosts = sizeof(hosts) / sizeof(hosts[0]);

int is_unwanted_char(char c)
{
	char unwanted_chars[] = { '\\', ' ', '\n', '\"', '\'' };

	for (int i = 0; i < strlen(unwanted_chars); i++)
	{
		if (c == unwanted_chars[i])
		{
			return 1;
		}
	}

	return 0;
}

void strip_unwanted_chars(char* str)
{
	int j = 0;
	for (int i = 0; i < strlen(str); i++)
	{
		if (is_unwanted_char(str[i]))
		{
			continue;
		}

		str[j] = str[i];
		j++;
	}

	str[j] = 0;
}

char* regular_response(char* response)
{
	char* result = malloc(sizeof(char) * strlen(response));
	strcpy(result, response);
	free(response);
	strip_unwanted_chars(result);
	return result;
}

char* get_json_url(char* response)
{
	char* start = strstr(response, "url\":") + 6;
	char* end = strstr(start, ".jpg") + 4;
	*end = 0;
	int len = (int)(end - start);
	char* result = malloc(sizeof(char) * len);
	strcpy(result, start);
	free(response);
	strip_unwanted_chars(result);
	result[len] = 0;
	return result;
}

char* pomf_cat(char* response)
{
	char* append = get_json_url(response);
	const char* base = "https://a.pomf.cat/";
	int len = (int)(strlen(base) + strlen(append));
	char* result = malloc((size_t)len + 1);
	memcpy(result, base, strlen(base));
	memcpy(result + strlen(base), append, strlen(append) + 1);
	return result;
}
