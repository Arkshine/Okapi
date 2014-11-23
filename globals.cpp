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

#include "globals.h"

GameLibrariesType       G_GameLibraries ={ NULL, NULL };
StringHashMap<Command*> G_Commands;
Memory                  G_Memory;
OffsetHandler*          G_OffsetHandler = NULL;
HL_TypeConversion       G_HL_TypeConversion;
FakemetaConstToAddress* G_FakemetaConstToAddress;
FunctionsDataMap        G_Functions;
Allocator               G_Allocator;
TypeHandler*            G_ArgsTypeHandler[9] ={ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

void list_commands()
{
	Util::con_printf("available commands:\n\n");

	for (StringHashMap<Command*>::iterator iter = G_Commands.iter(); !iter.empty(); iter.next())
	{
		if (iter->key.compare("help") == 0)
		{
			continue;
		}

		Util::con_printf("\t%s - %s\n", iter->key.chars(), iter->value->get_description());
	}
}
