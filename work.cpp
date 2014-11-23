//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// Okapi Module
//

#include <work.h>

void clean_debug_file()
{
	FILE* f = fopen("debug.txt", "w+");
	fclose(f);
}

void print(char* format, ...)
{
	char buffer[1000];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);

	SERVER_PRINT(buffer);
	Util::debug_fprintf(buffer);

	va_end(args);
}

void patch_printf()
{
#if defined __linux__
#else
	unsigned char* c = (unsigned char*)&printf;

	int a;
	VirtualProtect(c, 5, PAGE_EXECUTE_READWRITE, (PDWORD)&a);
	c[0] = 0xE9;
	*((long*)(&c[1])) = (char*)print - (char*)printf - 5;

#endif
}

//extern "C" void __cxa_pure_virtual() { while (1); }
