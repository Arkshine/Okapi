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

#include "s_library.h"
#include "globals.h"

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
			library->handle  = dlopen(info.name, RTLD_NOW);
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

void* find_function(s_library* library, const char* functionName)
{
	return g_MemUtils.ResolveSymbol(library->handle, functionName);
}

#if defined(LINUX) || defined(OSX)

	static int retrieve_addons_libraries(struct dl_phdr_info *info, size_t size, void *data)
	{
		char process[260];
		char library[32];

		strncpy(process, info->dlpi_name, strlen(info->dlpi_name));

		if (strstr(process, "addons"))
		{
			size_t length = strlen(process);
			size_t i = length;

			while (process[--i] != '.') {}; process[i] = '\0';
			while (process[--i] != '/') {};

			char *pLibrary = strcpy(library, &process[i + 1]);
			char *ptr = strstr(pLibrary, "_i386");

			if (ptr)
			{
				library[ptr - pLibrary] = '\0';
			}

			G_GameLibraries.Others.insert(library, new GameLibraryAny(*create_library((void *)info->dlpi_addr)));
		}

		return 0;
	}

#else

	void retrieve_addons_libraries() // IM THE KING OF EXAMPLE COPYPASTING!
	{
		HMODULE hMods[1024];
		HANDLE hProcess;
		DWORD cbNeeded;
		unsigned int i;

		hProcess = GetCurrentProcess();

		if (hProcess == NULL) // IS NOT POSSIBLE!
			return;

		if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
		{
			TCHAR process[MAX_PATH];
			char library[32];

			for (i = 0; i < (cbNeeded / sizeof(HMODULE)); ++i)
			{
				if (GetModuleFileNameEx(hProcess, hMods[i], process, sizeof(process) / sizeof(TCHAR)))
				{
					_MODULEINFO info;
					GetModuleInformation(hProcess, hMods[i], &info, sizeof(info));

					if (strstr(process, "addons"))
					{
						size_t length = strlen(process);
						size_t i = length;

						while (process[--i] != '.') {}; process[i] = '\0';
						while (process[--i] != '\\') {};

						strcpy(library, &process[i + 1]);

						G_GameLibraries.Others.insert(library, new GameLibraryAny(*create_library((void *)info.lpBaseOfDll)));
					}
				}
			}
		}
	}
#endif

void create_addons_libraries()
{
	G_GameLibraries.Others.insert("mod", G_GameLibraries.Mod);
	G_GameLibraries.Others.insert("engine", G_GameLibraries.Engine);

	#if defined(LINUX) || defined(OSX)
		dl_iterate_phdr(retrieve_addons_libraries, NULL);
	#else
		retrieve_addons_libraries();
	#endif
}