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

const char* FloatHandler::get_name()
{
	return "float";
}

ForwardParam FloatHandler::get_amx_param_type()
{
	return FP_CELL;
}

bool FloatHandler::uses_st_register()
{
	return true;
}

void FloatHandler::convert_to_amxx(Allocator& allocator, void** stack, cell& value)
{
	value = (cell)(*stack);
}

void FloatHandler::convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param)
{
	float* stack_f = (float*)stack;

	*stack_f = amx_ctof(*MF_GetAmxAddr(amx, param));
}

long FloatHandler::convert_from_amxx(Allocator& allocator, AMX *amx, cell* params)
{
	return amx_ctof(*MF_GetAmxAddr(amx, *params));
}

long FloatHandler::convert_to_amxx(AMX *amx, cell* params, long value)
{
	return amx_ftoc(value);
}
