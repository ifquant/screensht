#pragma once

typedef struct
{
	char* arg_name;
	char* upload_url;
	char* form_name;
	char* json_url_key;
	char* prefix;
} host_t;

char* parse_response(char* response);

const extern host_t hosts[];
const extern int n_hosts;
