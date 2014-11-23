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

const char* IntHandler::get_name()
{
	return "int";
}

ForwardParam IntHandler::get_amx_param_type()
{
	return FP_CELL;
}

void IntHandler::convert_to_amxx(Allocator& allocator, void** stack, cell& value)
{
	value = (cell)(*stack);
}

void IntHandler::convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param)
{
	*stack = (void*)*MF_GetAmxAddr(amx, param);
}

long IntHandler::convert_to_amxx(AMX *amx, cell* params, long value)
{
	return value;
}

long IntHandler::convert_from_amxx(Allocator& allocator, AMX *amx, cell* params)
{
	return (long)*MF_GetAmxAddr(amx, *params);
}
