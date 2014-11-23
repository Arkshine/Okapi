#include <offset_handler.h>
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

#include "offset_handler.h"
#include "globals.h"
#include <amxxmodule.h>

void OffsetHandler::search_pev()
{
	edict_t* edict = INDEXENT(0);
	entvars_t* entvars = &edict->v;

	char* private_c = (char*)edict->pvPrivateData;

	for (size_t i = 0; i < 0xFFF; ++i)
	{
		long val = *((long*)(private_c + i));

		if (val == (long)entvars)
		{
			this->PEV = i;

			return;
		}
	}

	Util::con_printf("PEV value was not found. This should not happen\n");

	this->PEV = 0;
}

void OffsetHandler::search_virtual_table()
{
	edict_t* edict = INDEXENT(0);
	entvars_t* entvars = &edict->v;

	this->EntityVirtualTable = search_virtual_table(edict->pvPrivateData);
}

size_t OffsetHandler::search_virtual_table(void* address)
{
	char* c_address = (char*)address;

	for (size_t i = 0; i < 0xFFF; ++i)
	{
		long address = *((long*)(c_address + i));

		if (G_GameLibraries.Engine->contains_address(address) || G_GameLibraries.Mod->contains_address(address))
		{
			void** address_ptr = (void**)address;

			size_t sum = 0;

			for (size_t j = 0; j <= 10; ++j)
			{
				void* address_inside = address_ptr[j];

				sum += G_GameLibraries.Engine->contains_address(address) | G_GameLibraries.Mod->contains_address(address);
			}

			if (sum > 5)
			{
				return i;
			}
		}
	}

	Util::con_printf("Virtual table was not found. This should not happen\n");

	return 0;
}
