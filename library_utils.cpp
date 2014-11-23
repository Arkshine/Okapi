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

#include "library_utils.h"
#include <MemoryUtils.h>

s_library* create_library(void* address)
{
	DynLibInfo info;
	s_library* library = NULL;

	memset(&info, 0, sizeof(DynLibInfo));

	if (g_MemUtils.GetLibraryInfo(address, info))
	{
		library = new s_library;

		library->address = info.baseAddress;
		library->length  = info.memorySize;

		#if defined(__linux__) || defined(__APPLE__)
			library->handle  = dlopen(info.dli_fname, RTLD_NOW);
		#else
			library->handle  = (void *)info.handle;
		#endif

		return library;
	}

	return library;
}

const char* get_address_symbol(void* address)
{
	#if defined(__linux__) || defined(__APPLE__)
		Dl_info info;

		if (dladdr(address, &info))
		{
			return info.dli_sname;
		}
	#endif

	return "";
}

void* find_function(s_library* library, const char* functionName, bool is_hidden)
{
	return g_MemUtils.ResolveSymbol(library->handle, functionName, is_hidden);
}
