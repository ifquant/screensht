#ifndef HOSTS_H
#define HOSTS_H

typedef struct
{
	char* arg_name;
	char* form_name;
	char* upload_url;
	char* (*convert_url)(char*);
} host_t;

char* regular_response(char* response);
char* get_json_url(char* response);
char* pomf_cat(char* response);

const extern host_t const hosts[];
const extern int n_hosts;

#endif
