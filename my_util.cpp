
#include <my_util.h>
#include "amxxmodule.h"

void Util::con_printf(char *format, ...)
{
	static char buffer[1000];
	va_list args;
	va_start (args, format);
	vsprintf (buffer,format, args);
	SERVER_PRINT(buffer);
	va_end (args);
}

void Util::debug_fprintf(char * format, ...)
{
	static char buffer[1000];
	va_list args;
	va_start (args, format);
	vsprintf (buffer,format, args);
	va_end (args);

	FILE* f = fopen("debug.txt","a+");
	fprintf(f,buffer);
	fclose(f);
}
