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
#include "globals.h"

const char* CBaseHandler::get_name()
{
	return "cbase";
}

ForwardParam CBaseHandler::get_amx_param_type()
{
	return FP_CELL;
}

void CBaseHandler::convert_to_amxx(Allocator& allocator, void** stack, cell& value)
{
	value = (cell)G_HL_TypeConversion.cbase_to_id((*stack));
}

void CBaseHandler::convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param)
{
	cell value = *MF_GetAmxAddr(amx, param);

	void* converted = G_HL_TypeConversion.id_to_cbase(value);

	*stack = (void*)converted;
}

long CBaseHandler::convert_from_amxx(Allocator& allocator, AMX *amx, cell* params)
{
	cell value = *MF_GetAmxAddr(amx, *params);

	return (long)G_HL_TypeConversion.id_to_cbase(value);
}

long CBaseHandler::convert_to_amxx(AMX *amx, cell* params, long value)
{
	return G_HL_TypeConversion.cbase_to_id((void*)value);
}
