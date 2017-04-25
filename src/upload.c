#include "upload.h"

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "hosts.h"

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

char* upload_sht(unsigned char* file_buffer, unsigned long size)
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
			arg_values.host.form_name,
			CURLFORM_BUFFER,
			arg_values.form_filename,
			CURLFORM_BUFFERPTR,
			file_buffer,
			CURLFORM_BUFFERLENGTH,
			(long)size,
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

	curl_easy_setopt(curl, CURLOPT_URL, arg_values.host.upload_url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memory_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&buffer);

	if (arg_values.verbose)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	}

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
		curl_multi_fdset(multi, &read, &write, &err, &max_fd);
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
	curl_global_cleanup();
	curl_formfree(post);

	char* tmp = strdup(buffer.data);
	free(buffer.data);

	return parse_response(tmp);
}
