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

const char* StrHandler::get_name()
{
	return "string";
}

ForwardParam StrHandler::get_amx_param_type()
{
	return FP_ARRAY;
}

int StrHandler::by_ref_n_params()
{
	return 2;
}

void StrHandler::convert_to_amxx(Allocator& allocator, void** stack, cell& value)
{
	char* str = (char*)*stack;

	if (str)
	{
		value = MF_PrepareCharArray((char*)str, strlen((char *)str) + 1);
	}
	else
	{
		value = MF_PrepareCharArray((char *)"", 1);
	}
}

void StrHandler::convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param)
{
	int len;
	const char *str = MF_GetAmxString(amx, param, 0, &len);
	char *allocString = allocator.alloc<char>(len + 1);
	strcpy(allocString, str);

	*stack = (void*)allocString;
}

long StrHandler::convert_from_amxx(Allocator& allocator, AMX *amx, cell* params)
{
	int len;
	const char *str = MF_GetAmxString(amx, *params, 0, &len);
	char *allocString = allocator.alloc<char>(len + 1);
	strcpy(allocString, str);

	return (long)allocString;
}

long StrHandler::convert_to_amxx(AMX *amx, cell* params, long value)
{
	MF_SetAmxString(amx, params[0], (const char *)value, *MF_GetAmxAddr(amx, params[1]));

	return 0;
}