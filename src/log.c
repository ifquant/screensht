#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include "args.h"

void log_info(const char* format, ...)
{
	if (!arg_values.verbose)
	{
		return;
	}

	va_list argptr;
	va_start(argptr, format);
	vfprintf(stdout, format, argptr);
	va_end(argptr);
}
