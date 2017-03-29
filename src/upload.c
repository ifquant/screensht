#include "upload.h"

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"

char* remove_newline(char* response)
{
	int len = (int)strlen(response) - 1;
	char* result = (char*)malloc(sizeof(char) * len);
	strncpy(result, response, (size_t)len);
	result[len] = 0; // remove newline
	free(response);
	return result;
}

char* strip_json(char* response)
{
	// mainly for pomf clones

	// cba to do json parsing
	char* start = strstr(response, "url\":") + 6;
	char* end = strstr(start, ".jpg") + 4;
	*end = 0;
	int len = (int)(end - start);
	char* result = (char*)malloc(sizeof(char) * len);

	int j = 0;
	for (int i = 0; i < len; i++)
	{
		if (start[i] == '\\'
		 || start[i] == ' '
		 || start[i] == '\"'
		 || start[i] == '\n')
		{
			continue;
		}

		result[j] = start[i];
		j++;
	}

	result[j] = 0;
	free(response);
	return result;
}

char* pomf_cat(char* response)
{
	char* append = strip_json(response);
	const char* base = "https://a.pomf.cat/";
	int len = (int)(strlen(base) + strlen(append));
	char* result = (char*)malloc((size_t)len + 1);
	memcpy(result, base, strlen(base));
	memcpy(result + strlen(base), append, strlen(append) + 1);
	return result;
}

char* do_nothing(char* response)
{
	char* result = (char*)malloc(sizeof(char) * strlen(response));
	strcpy(result, response);
	free(response);
	return result;
}

host_t hosts[] = {
		{ "0x0.st", "file", "https://0x0.st/", &remove_newline },
		{ "mixtape.moe", "files[]", "https://mixtape.moe/upload.php", &strip_json },
		{ "nya.is", "files[]", "https://nya.is/upload", &strip_json },
		{ "p.fuwafuwa.moe", "files[]", "https://p.fuwafuwa.moe/upload.php", &strip_json },
		{ "safe.moe", "files[]", "https://safe.moe/api/upload", &strip_json },
		{ "cocaine.ninja", "files[]", "https://cocaine.ninja/upload.php?output=text", &remove_newline },
		{ "comfy.moe", "files[]", "https://comfy.moe/upload.php", &strip_json },
		{ "pomf.cat", "files[]", "https://pomf.cat/upload.php", &pomf_cat },
		{ "lewd.se", "file", "https://lewd.se/api.php?d=upload-tool", &do_nothing },
		{ "memenet.org", "files[]", "https://memenet.org/upload.php", &strip_json },
		{ "uguu.se", "file", "https://uguu.se/api.php?d=upload-tool", &do_nothing },
		{ "yiff.moe", "files[]", "https://yiff.moe/upload.php", &strip_json },
		{ "vidga.me", "files[]", "https://vidga.me/upload.php", &strip_json }
};

struct string
{
	char* data;
	size_t size;
};

static size_t memory_callback(void* content, size_t size, size_t nmemb, void* userp)
{
	size_t real_size = size * nmemb;
	struct string* mem = (struct string*)userp;

	mem->data = realloc(mem->data, mem->size + real_size + 1);
	memcpy(&mem->data[mem->size], content, real_size);
	mem->size += real_size;
	mem->data[mem->size] = 0;

	return real_size;
}

char* upload_sht(char* filename)
{
	struct curl_httppost* post = 0;
	struct curl_httppost* lastptr = 0;

	struct string buffer;
	buffer.data = malloc(1);
	buffer.size = 0;

	curl_formadd(
			&post,
			&lastptr,
			CURLFORM_COPYNAME,
			args.host.form_name,
			CURLFORM_FILE,
			filename,
			CURLFORM_END
	);

	CURL* curl = curl_easy_init();
	CURLM* multi = curl_multi_init();

	if (!curl || !multi)
	{
		printf("curl initialization failed\n");
		curl_multi_cleanup(multi);
		curl_easy_cleanup(curl);
		curl_formfree(post);
		return buffer.data;
	}

	curl_easy_setopt(curl, CURLOPT_URL, args.host.upload_url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memory_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&buffer);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);

	curl_multi_add_handle(multi, curl);
	int running;
	curl_multi_perform(multi, &running);

	while (running)
	{
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		long curl_timeout = -1;

		curl_multi_timeout(multi, &curl_timeout);

		if (curl_timeout >= 0)
		{
			timeout.tv_sec = curl_timeout / 1000;
			if (timeout.tv_sec > 1)
			{
				timeout.tv_sec = 1;
			}
			else
			{
				timeout.tv_usec = (curl_timeout % 1000) * 1000;
			}
		}

		fd_set read;
		fd_set write;
		fd_set err;

		FD_ZERO(&read);
		FD_ZERO(&write);
		FD_ZERO(&err);

		int max_fd = -1;
		CURLMcode multi_code = curl_multi_fdset(multi, &read, &write, &err, &max_fd);
		int select_code;

		if (max_fd == -1)
		{
			struct timeval wait = { 0, 100000 };
			select_code = select(0, 0, 0, 0, &wait);
		}
		else
		{
			select_code = select(max_fd + 1, &read, &write, &err, &timeout);
		}

		if (select_code != -1)
		{
			curl_multi_perform(multi, &running);
		}
	}

	curl_multi_cleanup(multi);
	curl_easy_cleanup(curl);
	curl_formfree(post);

	printf("%s\n", buffer.data);

	return args.host.convert_url(buffer.data);
}
