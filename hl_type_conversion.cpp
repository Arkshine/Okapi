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

#include <hl_type_conversion.h>
#include <globals.h>

entvars_t* HL_TypeConversion::cbase_to_edict(void* cbase)
{
	if(!cbase)
		return NULL;

	return *(entvars_t **)((char *)(cbase) + G_OffsetHandler->PEV);
}

	