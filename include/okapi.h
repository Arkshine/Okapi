/* AMX Mod X
*   Fun Module
*
* by the AMX Mod X Development Team
*
* This file is part of AMX Mod X.
*
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 2 of the License, or (at
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software Foundation,
*  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
*  In addition, as a special exception, the author gives permission to
*  link the code of this program with the Half-Life Game Engine ("HL
*  Engine") and Modified Game Libraries ("MODs") developed by Valve,
*  L.L.C ("Valve"). You must obey the GNU General Public License in all
*  respects for all of the code used other than the HL Engine and MODs
*  from Valve. If you modify this file, you may extend this exception
*  to your version of the file, but you are not obligated to do so. If
*  you do not wish to do so, delete this exception statement from your
*  version.
*/
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
