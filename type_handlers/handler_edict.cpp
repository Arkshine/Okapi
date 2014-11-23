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

#include "type_handler.h"

const char* EdictHandler::get_name()
{
	return "edict";
}

ForwardParam EdictHandler::get_amx_param_type()
{
	return FP_CELL;
}

void EdictHandler::convert_to_amxx(Allocator& allocator, void** stack, cell& value)
{
	value = (cell)ENTINDEX((edict_t*)*stack);
}

void EdictHandler::convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param)
{
	cell value = *MF_GetAmxAddr(amx, param);

	void* converted = INDEXENT(value);

	*stack = (void*)converted;
}

long EdictHandler::convert_from_amxx(Allocator& allocator, AMX *amx, cell* params)
{
	cell value = *MF_GetAmxAddr(amx, *params);

	void* converted = INDEXENT(value);

	return (long)converted;
}

long EdictHandler::convert_to_amxx(AMX *amx, cell* params, long value)
{
	return (long)ENTINDEX((edict_t*)value);
}