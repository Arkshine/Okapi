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

#ifndef TYPE_HANDLER
#define TYPE_HANDLER

#include <allocator.h>
#include "assembly_create.h"
#include "amxxmodule.h"

class TypeHandler
{
	public:

		virtual int stack_places()
		{
			return 1;
		}

		virtual bool uses_st_register()
		{
			return 0;
		}

		virtual int by_ref_n_params()
		{
			return 0;
		}

		virtual long convert_from_amxx(Allocator& allocator, AMX *amx, cell* params) = 0;

		virtual long convert_to_amxx(AMX *amx, cell* params, long value) = 0;

		virtual ForwardParam get_amx_param_type() = 0;

		virtual const char* get_name() = 0;

		virtual void convert_to_amxx(Allocator& allocator, void** stack, cell& value) = 0;

		virtual void convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param) = 0;
};

class IntHandler : public TypeHandler
{
	const char* get_name();

	ForwardParam get_amx_param_type();

	void convert_to_amxx(Allocator& allocator, void** stack, cell& value);
	void convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param);

	long convert_to_amxx(AMX *amx, cell* params, long value);
	long convert_from_amxx(Allocator& allocator, AMX *amx, cell* params);
};

class VoidHandler : public TypeHandler
{
	const char* get_name();

	ForwardParam get_amx_param_type();

	void convert_to_amxx(Allocator& allocator, void** stack, cell& value);
	void convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param);

	long convert_to_amxx(AMX *amx, cell* params, long value);
	long convert_from_amxx(Allocator& allocator, AMX *amx, cell* params);
};

class CBaseHandler : public TypeHandler
{
	const char* get_name();

	ForwardParam get_amx_param_type();

	void convert_to_amxx(Allocator& allocator, void** stack, cell& value);
	void convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param);

	long convert_to_amxx(AMX *amx, cell* params, long value);
	long convert_from_amxx(Allocator& allocator, AMX *amx, cell* params);
};

class FloatHandler : public TypeHandler
{
	const char* get_name();

	ForwardParam get_amx_param_type();

	bool uses_st_register();

	void convert_to_amxx(Allocator& allocator, void** stack, cell& value);
	void convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param);

	long convert_to_amxx(AMX *amx, cell* params, long value);
	long convert_from_amxx(Allocator& allocator, AMX *amx, cell* params);
};

class VecHandler : public TypeHandler
{
	const char* get_name();

	ForwardParam get_amx_param_type();

	int by_ref_n_params();
	int stack_places();

	void convert_to_amxx(Allocator& allocator, void** stack, cell& value);
	void convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param);

	long convert_to_amxx(AMX *amx, cell* params, long value);
	long convert_from_amxx(Allocator& allocator, AMX *amx, cell* params);
};

class EntvarsHandler : public TypeHandler
{
	const char* get_name();

	ForwardParam get_amx_param_type();

	void convert_to_amxx(Allocator& allocator, void** stack, cell& value);
	void convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param);

	long convert_to_amxx(AMX *amx, cell* params, long value);
	long convert_from_amxx(Allocator& allocator, AMX *amx, cell* params);
};

class StrHandler : public TypeHandler
{
	const char* get_name();

	ForwardParam get_amx_param_type();

	int by_ref_n_params();

	void convert_to_amxx(Allocator& allocator, void** stack, cell& value);
	void convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param);

	long convert_to_amxx(AMX *amx, cell* params, long value);
	long convert_from_amxx(Allocator& allocator, AMX *amx, cell* params);
};

class EdictHandler : public TypeHandler
{
	const char* get_name();

	ForwardParam get_amx_param_type();

	void convert_to_amxx(Allocator& allocator, void** stack, cell& value);
	void convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param);

	long convert_to_amxx(AMX *amx, cell* params, long value);
	long convert_from_amxx(Allocator& allocator, AMX *amx, cell* params);
};

class VecPtrHandler : public TypeHandler
{
	const char* get_name();

	ForwardParam get_amx_param_type();

	int by_ref_n_params();

	void convert_to_amxx(Allocator& allocator, void** stack, cell& value);
	void convert_from_amxx(Allocator& allocator, void** stack, AMX* amx, cell param);

	long convert_to_amxx(AMX *amx, cell* params, long value);
	long convert_from_amxx(Allocator& allocator, AMX *amx, cell* params);
};

#endif // TYPE_HANDLER

