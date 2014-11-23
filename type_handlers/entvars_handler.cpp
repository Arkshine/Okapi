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

const char* EntvarsHandler::get_name()
{
	return "entvars";
}

ForwardParam EntvarsHandler::get_amx_param_type()
{
	return FP_CELL;
}

void EntvarsHandler::convert_to_amxx(Allocator& allocator, void** stack, cell& value)
{
	value = (cell)(G_HL_TypeConversion.entvar_to_id((entvars_t*)*stack));
}

void EntvarsHandler::convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param)
{
	cell value = *MF_GetAmxAddr(amx, param);

	void* converted = G_HL_TypeConversion.id_to_entvar(value);

	*stack = (void*)converted;
}

long EntvarsHandler::convert_from_amxx(Allocator& allocator, AMX *amx, cell* params)
{
	cell value = *MF_GetAmxAddr(amx, *params);

	void* converted = G_HL_TypeConversion.id_to_entvar(value);

	return (long)converted;
}

long EntvarsHandler::convert_to_amxx(AMX *amx, cell* params, long value)
{
	return (long)G_HL_TypeConversion.entvar_to_id((entvars_t*)value);
}