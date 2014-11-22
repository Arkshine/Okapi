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

#include "amxxmodule.h"
#include <game_library.h>
#include <globals.h>

template <class T>
long library_find(GameLibrary* library, long address, T value)
{
	long end = (long)library->address + (long)library->length + 1 - (sizeof(T));

	while (address < end)
	{
		T* i_address = (T*)address;

		if (*i_address == value)
		{
			return address;
		}

		address++;
	}

	return 0;
}

template <class T>
int library_replace(GameLibrary* library, T val_search, T val_replace)
{
	long start = (long)library->address;

	int count = 0;

	for (long i=0; i < library->length + 1 - (long)sizeof(T); i++)
	{
		T* address = (T*)(((char*)start) + i);

		if (*address == val_search)
		{
			int protection = G_Memory.get_memory_protection((long)address);

			G_Memory.make_writable((long)address);

			*address = val_replace;

			count++;

			G_Memory.set_memory_protection((long)address, protection);
		}
	}

	return count;
}
