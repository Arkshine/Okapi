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

const char* VecPtrHandler::get_name()
{
	return "vec_ptr";
}

ForwardParam VecPtrHandler::get_amx_param_type()
{
	return FP_ARRAY;
}

int VecPtrHandler::by_ref_n_params()
{
	return 1;

}
void VecPtrHandler::convert_to_amxx(Allocator& allocator, void** stack, cell& value)
{
	long* stack_l = (long*)stack[0];

	cell *amxVector = allocator.alloc<cell>(3);

	amxVector[0] = ((cell)(stack_l[0]));
	amxVector[1] = ((cell)(stack_l[1]));
	amxVector[2] = ((cell)(stack_l[2]));

	value = MF_PrepareCellArray(amxVector, 3);
}

void VecPtrHandler::convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param)
{
	Vector*& vec = (Vector*&)*stack;

	vec = allocator.alloc<Vector>(1);

	cell* amxx_vector = MF_GetAmxAddr(amx, param);

	vec->x = amx_ctof(amxx_vector[0]);
	vec->y = amx_ctof(amxx_vector[1]);
	vec->z = amx_ctof(amxx_vector[2]);
}

long VecPtrHandler::convert_from_amxx(Allocator& allocator, AMX *amx, cell* params)
{
	cell* amxx_vector = MF_GetAmxAddr(amx, *params);

	Vector* vec = allocator.alloc<Vector>(1);

	vec->x = amx_ctof(amxx_vector[0]);
	vec->y = amx_ctof(amxx_vector[1]);
	vec->z = amx_ctof(amxx_vector[2]);

	return (long)vec;
}

long VecPtrHandler::convert_to_amxx(AMX *amx, cell* params, long value)
{
	cell* amxx_vector = MF_GetAmxAddr(amx, *params);

	Vector* vec = (Vector*)value;

	amxx_vector[0] = amx_ftoc(vec->x);
	amxx_vector[1] = amx_ftoc(vec->y);
	amxx_vector[2] = amx_ftoc(vec->z);

	return 0;
}