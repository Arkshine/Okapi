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

#include "utils.h"
#include "amxxmodule.h"

void Util::con_printf(const char *format, ...)
{
	static char buffer[1000];

	va_list args;
	va_start(args, format);

	vsprintf(buffer, format, args);
	SERVER_PRINT(buffer);

	va_end(args);
}

void Util::debug_fprintf(const char * format, ...)
{
	static char buffer[1000];

	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);

	FILE* f = fopen("debug.txt", "a+");
	fprintf(f, "%s", buffer);
	fclose(f);
}
