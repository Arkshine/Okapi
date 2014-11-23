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

const char* VoidHandler::get_name()
{
	return "void";
}

ForwardParam VoidHandler::get_amx_param_type()
{
	return FP_CELL;
}

void VoidHandler::convert_to_amxx(Allocator& allocator, void** stack, cell& value)
{
}

void VoidHandler::convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param)
{
}

long VoidHandler::convert_from_amxx(Allocator& allocator, AMX *amx, cell* params)
{
	return 0;
}

long VoidHandler::convert_to_amxx(AMX *amx, cell* params, long value)
{
	return 0;
}
