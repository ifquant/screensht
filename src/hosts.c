#include "hosts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jsmn/jsmn.h"
#include "args.h"
#include "log.h"

const host_t hosts[] = {
		{
				.arg_name = "0x0.st",
				.upload_url = "https://0x0.st/",
				.form_name = "file"
		},
		{
				.arg_name = "mixtape.moe",
				.upload_url = "https://mixtape.moe/upload.php",
				.form_name = "files[]",
				.json_url_key = "url"
		},
		{
				.arg_name = "nya.is",
				.upload_url = "https://nya.is/upload",
				.form_name = "files[]",
				.json_url_key = "url"
		},
		{
				.arg_name = "p.fuwafuwa.moe",
				.upload_url = "https://p.fuwafuwa.moe/upload.php",
				.form_name = "files[]",
				.json_url_key = "url"
		},
		{
				.arg_name = "safe.moe",
				.upload_url = "https://safe.moe/api/upload",
				.form_name = "files[]",
				.json_url_key = "url"
		},
		{
				.arg_name = "cocaine.ninja",
				.upload_url = "https://cocaine.ninja/upload.php?output=text",
				.form_name = "files[]"
		},
		{
				.arg_name = "comfy.moe",
				.upload_url = "https://comfy.moe/upload.php",
				.form_name = "files[]",
				.json_url_key = "url"
		},
		{
				.arg_name = "pomf.cat",
				.upload_url = "https://pomf.cat/upload.php",
				.form_name = "files[]",
				.json_url_key = "url",
				.prefix = "https://a.pomf.cat/"
		},
		{
				.arg_name = "lewd.se",
				.upload_url = "https://lewd.se/api.php?d=upload-tool",
				.form_name = "file"
		},
		{
				.arg_name = "memenet.org",
				.upload_url = "https://memenet.org/upload.php",
				.form_name = "files[]",
				.json_url_key = "url"
		},
		{
				.arg_name = "uguu.se",
				.upload_url = "https://uguu.se/api.php?d=upload-tool",
				.form_name = "file"
		},
		{
				.arg_name = "yiff.moe",
				.upload_url = "https://yiff.moe/upload.php",
				.form_name = "files[]",
				.json_url_key = "url"
		},
		{
				.arg_name = "vidga.me",
				.upload_url = "https://vidga.me/upload.php",
				.form_name = "files[]",
				.json_url_key = "url"
		},
//		{
//				.arg_name = "aww.moe",
//				.upload_url = "https://u.aww.moe/upload",
//				.form_name = "files[]",
//				.json_url_key = "fullurl"
//		},
		{
				.arg_name = "pomf.space",
				.upload_url = "https://pomf.space/upload.php",
				.form_name = "files[]",
				.json_url_key = "url"
		},
		{
				.arg_name = "reich.io",
				.upload_url = "https://reich.io/upload.php",
				.form_name = "files[]",
				.json_url_key = "url"
		},
		{
				.arg_name = "sugoi.vidyagam.es",
				.upload_url = "https://sugoi.vidyagam.es/upload.php",
				.form_name = "files[]",
				.json_url_key = "url"
		},
		{
				.arg_name = "up.che.moe",
				.upload_url = "http://up.che.moe/upload.php",
				.form_name = "files[]",
				.json_url_key = "url"
		}
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

jsmntok_t* get_tokens(char* json, int* n_tokens)
{
	jsmn_parser parser;
	jsmn_init(&parser);

	int size = 32;
	jsmntok_t* tokens = malloc(sizeof(jsmntok_t) * size);
	int code = jsmn_parse(&parser, json, strlen(json), tokens, (unsigned int)size);

	while (code == JSMN_ERROR_NOMEM)
	{
		log_info("expanding tokens buffer from %d to ", size);
		size *= 2;
		log_info("%d\n", size);
		tokens = realloc(tokens, sizeof(jsmntok_t) * size);
		code = jsmn_parse(&parser, json, strlen(json), tokens, (unsigned int)size);
	}

	if (code == JSMN_ERROR_INVAL)
	{
		printf("invalid json response\n");
	}

	*n_tokens = size;

	return tokens;
}

char* get_text_by_key(char *json, char* key)
{
	int n_tokens = 0;
	jsmntok_t* tokens = get_tokens(json, &n_tokens);

	log_info("%d tokens allocated\n", n_tokens);

	for (int i = 0; i < n_tokens; i++)
	{
		jsmntok_t *curr = &tokens[i];

		if (!strncmp(key, &json[curr->start], (size_t)(curr->end - curr->start)))
		{
			curr++;
			return strndup(&json[curr->start], (size_t)(curr->end - curr->start));
		}
	}

	printf("couldn't find \"%s\" in json response\n", key);

	return 0;
}

char* parse_response(char* response)
{
	log_info("raw:\n%s\n", response);

	char* result = malloc(strlen(response));
	char* url_key = arg_values.host.json_url_key;

	if (!url_key)
	{
		strcpy(result, response);
	}
	else
	{
		char* url_text = get_text_by_key(response, url_key);
		strcpy(result, url_text);
		free(url_text);
	}

	char* prefix = arg_values.host.prefix;

	if (prefix)
	{
		char* temp = strdup(result);
		result = realloc(result, strlen(prefix) + strlen(result));
		strcpy(result, prefix);
		strcat(result, temp);
		free(temp);
	}

	free(response);
	strip_unwanted_chars(result);

	log_info("parsed:\n%s\n", result);

	return result;
}
