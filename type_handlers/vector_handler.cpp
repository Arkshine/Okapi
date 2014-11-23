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

const char* VecHandler::get_name()
{
	return "vec";
}

ForwardParam VecHandler::get_amx_param_type()
{
	return FP_ARRAY;
}

int VecHandler::by_ref_n_params()
{
	return 1;
}

int VecHandler::stack_places()
{
	#if defined(LINUX) || defined(OSX)
		return 1;
	#else
		return 3;
	#endif
}

void VecHandler::convert_to_amxx(Allocator& allocator, void** stack, cell& value)
{
	#if defined(LINUX) || defined(OSX)
		long* stack_l = (long*)stack[0];
	#else
		long* stack_l = (long*)stack;
	#endif

	cell *amxVector = allocator.alloc<cell>(3);

	amxVector[0] = ((cell)(stack_l[0]));
	amxVector[1] = ((cell)(stack_l[1]));
	amxVector[2] = ((cell)(stack_l[2]));

	value = MF_PrepareCellArray(amxVector, 3);
}

void VecHandler::convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param)
{
	#if defined(LINUX) || defined(OSX)
		*stack = (float*)allocator.alloc<Vector>(1);

		float* stack_f = (float*)*stack;
	#else
		float* stack_f = (float*)stack;
	#endif

	cell* amxx_vector = MF_GetAmxAddr(amx, param);

	stack_f[0] = amx_ctof(amxx_vector[0]);
	stack_f[1] = amx_ctof(amxx_vector[1]);
	stack_f[2] = amx_ctof(amxx_vector[2]);
}

long VecHandler::convert_from_amxx(Allocator& allocator, AMX *amx, cell* params)
{
	cell* amxx_vector = MF_GetAmxAddr(amx, *params);

	Vector* vec = allocator.alloc<Vector>(1);

	vec->x = amx_ctof(amxx_vector[0]);
	vec->y = amx_ctof(amxx_vector[1]);
	vec->z = amx_ctof(amxx_vector[2]);

	return (long)vec;
}

long VecHandler::convert_to_amxx(AMX *amx, cell* params, long value)
{
	cell* amxx_vector = MF_GetAmxAddr(amx, *params);

	Vector* vec = (Vector*)value;

	amxx_vector[0] = amx_ftoc(vec->x);
	amxx_vector[1] = amx_ftoc(vec->y);
	amxx_vector[2] = amx_ftoc(vec->z);

	return 0;
}