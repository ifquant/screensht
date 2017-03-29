#ifndef UPLOAD_H
#define UPLOAD_H

typedef struct
{
	char* arg_name;
	char* form_name;
	char* upload_url;
	char* (*convert_url)(char*);
} host_t;

char* upload_sht(char* filename);

extern host_t hosts[13];

#endif
