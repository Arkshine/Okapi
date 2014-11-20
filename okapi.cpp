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

#include <okapi.h>
#include <templates.h>
#include <my_util.h>
#include <work.h>
#include <stdio.h>
#include <CVector.h>
#include <heap.h>
#include <trie.h>
#include <s_library.h>
#include <string_.h>
#include <trie_string.h>
#include <globals.h>
#include <game_libraries/game_library_engine.h>
#include <game_libraries/game_library_mod.h>
#include <commands/command_help.h>
#include <function.h>
#include <type_handlers/type_handler_extra.h>

using Util::con_printf;

static cell AMX_NATIVE_CALL wl(AMX *amx, cell *params)
{
#ifdef __linux__
	return params[2];
#else
	return params[1];
#endif
}

static cell AMX_NATIVE_CALL okapi_get_ptr_vec(AMX *amx, cell *params)
{
	long ptr = params[1];

	cell* amx_vec = MF_GetAmxAddr(amx, params[2]);

	Vector* vec = (Vector*)ptr;

	amx_vec[0] = amx_ftoc(vec->x);
	amx_vec[1] = amx_ftoc(vec->y);
	amx_vec[2] = amx_ftoc(vec->z);

	return 1;
}

static cell AMX_NATIVE_CALL okapi_alloc(AMX *amx, cell *params)
{
	int n = params[1];

	cell all = (cell)G_Allocator.alloc<int>(n);

	return all;
}

static cell AMX_NATIVE_CALL okapi_call(AMX *amx, cell *params)
{
	Function* function = (Function*)params[1];

	int params_n = params[0] / sizeof(cell);

	int expected_params_n = 1 + function->call_n_params();

	if (params_n != expected_params_n)
	{
		String s;

		s.sprintf("Invalid parameter count. Expecting %d parameters", expected_params_n);

		MF_LogError(amx, AMX_ERR_NATIVE, s.get_ptr());

		return 0;
	}

	return function->call_amx(amx, &params[2], false);
}

static cell AMX_NATIVE_CALL okapi_call_ex(AMX *amx, cell *params)
{
	Function* function = (Function*)params[1];

	int params_n = params[0] / sizeof(cell);

	int expected_params_n = 1 + function->call_n_params();

	if (params_n != expected_params_n)
	{
		String s;

		s.sprintf("Invalid parameter count. Expecting %d parameters", expected_params_n);

		MF_LogError(amx, AMX_ERR_NATIVE, s.get_ptr());

		return 0;
	}

	return function->call_amx(amx, &params[2], true);
}

static cell AMX_NATIVE_CALL okapi_set_param(AMX *amx, cell *params)
{
	AMX_Hook* hook = Function::get_current_hook();

	if (!hook)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Not inside a hook");

		return 0;
	}

	int params_n = params[0] / sizeof(cell);

	if (params_n != 2)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter count. Expecting two parameters");

		return 0;
	}

	int param_id = params[1] - 1;

	Function* function = hook->function;

	if ((param_id < 0) || (param_id >= function->get_n_args()))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter id");

		return 0;
	}

	function->set_arg(param_id, amx, params[2]);

	return 0;
}

static cell AMX_NATIVE_CALL okapi_get_orig_return(AMX *amx, cell *params)
{
	AMX_Hook* hook = Function::get_current_hook();

	if (!hook)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Not inside a hook");

		return 0;
	}

	if (hook->phase == 0)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Get return only available in hooks post call");

		return 0;
	}

	int params_n = params[0] / sizeof(cell);

	Function* function = hook->function;

	int expected_params_n = function->get_n_ret_params();

	if (params_n != expected_params_n)
	{
		String s;

		s.sprintf("Invalid parameter count. Expecting %d parameters", expected_params_n);

		MF_LogError(amx, AMX_ERR_NATIVE, s.get_ptr());

		return 0;
	}

	return function->get_origin_return(amx, &params[1]);
}

static cell AMX_NATIVE_CALL okapi_set_return(AMX *amx, cell *params)
{
	AMX_Hook* hook = Function::get_current_hook();

	if (!hook)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Not inside a hook");

		return 0;
	}

	if (hook->phase == 1)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Set return only available in hooks pre call");

		return 0;
	}

	int params_n = params[0] / sizeof(cell);

	if (params_n != 1)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter count. Expecting two parameters");

		return 0;
	}

	Function* function = hook->function;

	function->set_return(amx, &params[1]);

	return 1;
}

static cell AMX_NATIVE_CALL okapi_current_hook(AMX *amx, cell *params)
{
	AMX_Hook* hook = Function::get_current_hook();

	if (!hook)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Not inside a hook");

		return 0;
	}

	return (cell)hook;
}

static cell AMX_NATIVE_CALL okapi_del_hook(AMX *amx, cell *params)
{
	AMX_Hook*& hook = (AMX_Hook*&)*MF_GetAmxAddr(amx, params[1]);

	if (!hook)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "No hook provided, or already deleted");

		return 0;
	}

	Function* function = hook->function;

	function->del_hook(hook);

	hook = 0;

	return 1;
}

static cell AMX_NATIVE_CALL okapi_del_current_hook(AMX *amx, cell *params)
{
	AMX_Hook* hook = Function::get_current_hook();

	if (!hook)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "No hook provided, or already deleted");

		return 0;
	}

	Function* function = hook->function;

	function->del_hook(hook);

	return 1;
}

static cell AMX_NATIVE_CALL okapi_add_hook(AMX *amx, cell *params)
{
	Function* function = (Function*)params[1];

	char* callback = g_fn_GetAmxString(amx, params[2], 0, NULL);

	cell hook = (cell)function->add_hook(amx, callback, !!params[3]);

	if (!hook)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "The callback does not exist");
		return 0;
	}

	return hook;
}

static cell AMX_NATIVE_CALL okapi_build_method(AMX *amx, cell *params)
{
	int params_n = params[0] / sizeof(cell);

	long address = params[1];

	if (address == 0)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Null address provided");

		return 0;
	}

	Function*& function = G_Functions[address];

	if (function)
		return (cell)function;

	TypeHandler* return_handler = G_ArgsTypeHandler[params[2]];
	CVector<TypeHandler*> arguments_handlers;

	arguments_handlers.push_back(G_ArgsTypeHandler[params[3]]);

	for (int i=4; i <= params_n; i++)
	{
		int id = *MF_GetAmxAddr(amx, params[i]);
		arguments_handlers.push_back(G_ArgsTypeHandler[id]);
	}

	function = new FunctionMethod((void*)address, arguments_handlers, return_handler);

	return (cell)function;
}

static cell AMX_NATIVE_CALL okapi_build_vfunc_ptr(AMX *amx, cell *params)
{
	int params_n = params[0] / sizeof(cell);

	void* address = (void*)params[1];

	if (!address)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Null address provided");

		return 0;
	}

	int offset = params[2];

	void **vtable = *((void***)(((char*)address) + G_OffsetHandler->search_virtual_table(address)));

	void** entry_address = &vtable[offset];

	Function*& function = G_Functions[(long)entry_address];

	if (function)
	{
		return (cell)function;
	}

	TypeHandler* return_handler = G_ArgsTypeHandler[params[3]];
	CVector<TypeHandler*> arguments_handlers;

	arguments_handlers.push_back(G_ArgsTypeHandler[ArgInt]);

	for (int i=4; i <= params_n; i++)
	{
		int id = *MF_GetAmxAddr(amx, params[i]);
		arguments_handlers.push_back(G_ArgsTypeHandler[id]);
	}

	function = new FunctionVirtual((void*)entry_address, arguments_handlers, return_handler);

	return (cell)function;
}

static cell AMX_NATIVE_CALL okapi_build_vfunc_cbase(AMX *amx, cell *params)
{
	int params_n = params[0] / sizeof(cell);

	int id = params[1];

	if (ENTINDEX(INDEXENT(id)) != id)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid entity provided");

		return 0;
	}

	int offset = params[2];

	void* cbase = G_HL_TypeConversion.id_to_cbase(id);

	void **vtable = *((void***)(((char*)cbase) + G_OffsetHandler->EntityVirtualTable));

	void** entry_address = &vtable[offset];

	Function*& function = G_Functions[(long)entry_address];

	if (function)
	{
		return (cell)function;
	}

	TypeHandler* return_handler = G_ArgsTypeHandler[params[3]];
	CVector<TypeHandler*> arguments_handlers;

	arguments_handlers.push_back(G_ArgsTypeHandler[ArgCBase]);

	for (int i=4; i <= params_n; i++)
	{
		int id = *MF_GetAmxAddr(amx, params[i]);
		arguments_handlers.push_back(G_ArgsTypeHandler[id]);
	}

	function = new FunctionVirtual((void*)entry_address, arguments_handlers, return_handler);

	return (cell)function;
}

static cell AMX_NATIVE_CALL okapi_build_vfunc_class(AMX *amx, cell *params)
{
	int params_n = params[0] / sizeof(cell);

	char* classname = g_fn_GetAmxString(amx, params[1], 0, NULL);
	int offset = params[2];

	/* Code from hamsandwich */
	edict_t *Entity = CREATE_ENTITY();

	CALL_GAME_ENTITY(PLID, classname, &Entity->v);

	if (Entity->pvPrivateData == NULL)
	{
		REMOVE_ENTITY(Entity);

		MF_LogError(amx, AMX_ERR_NATIVE, "Failed to retrieve classtype for '%s'", classname);

		return 0;
	}

	void **vtable = *((void***)(((char*)Entity->pvPrivateData) + G_OffsetHandler->EntityVirtualTable));

	REMOVE_ENTITY(Entity);
	/* Code from hamsandwich */

	void** entry_address = &vtable[offset];

	Function*& function = G_Functions[(long)entry_address];

	if (function)
	{
		return (cell)function;
	}

	TypeHandler* return_handler = G_ArgsTypeHandler[params[3]];
	CVector<TypeHandler*> arguments_handlers;

	arguments_handlers.push_back(G_ArgsTypeHandler[ArgCBase]);

	for (int i=4; i <= params_n; i++)
	{
		int id = *MF_GetAmxAddr(amx, params[i]);
		arguments_handlers.push_back(G_ArgsTypeHandler[id]);
	}

	function = new FunctionVirtual((void*)entry_address, arguments_handlers, return_handler);

	return (cell)function;
}

static cell AMX_NATIVE_CALL okapi_build_function(AMX *amx, cell *params)
{
	int params_n = params[0] / sizeof(cell);

	long address = params[1];

	if (address == 0)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Null address provided");

		return 0;
	}

	Function*& function = G_Functions[address];

	if (function)
		return (cell)function;

	TypeHandler* return_handler = G_ArgsTypeHandler[params[2]];
	CVector<TypeHandler*> arguments_handlers;

	for (int i=3; i <= params_n; i++)
	{
		int id = *MF_GetAmxAddr(amx, params[i]);
		arguments_handlers.push_back(G_ArgsTypeHandler[id]);
	}

	function = new FunctionCdecl((void*)address, arguments_handlers, return_handler);

	return (cell)function;
}

static cell AMX_NATIVE_CALL okapi_get_engfunc_ptr_by_offset(AMX *amx, cell *params)
{
	return (cell)((void**)(&g_engfuncs))[params[1]];
}

static cell AMX_NATIVE_CALL okapi_get_dllfunc_ptr_by_offset(AMX *amx, cell *params)
{
	return (cell)((void**)(gpGamedllFuncs->dllapi_table))[params[1]];
}

static cell AMX_NATIVE_CALL okapi_get_engfunc_ptr(AMX *amx, cell *params)
{
	int id = params[1];

	if ((params[1] < 0) || (params[1] > EngFunc_ServerPrint))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid engfunc id");

		return 0;
	}

	return (cell)G_FakemetaConstToAddress->engfunc_addresses[id];
}

static cell AMX_NATIVE_CALL okapi_get_dllfunc_ptr(AMX *amx, cell *params)
{
	int id = params[1];

	if ((params[1] < 0) || (params[1] > DLLFunc_CreateBaseline))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid dllfunc id");

		return 0;
	}

	return (cell)G_FakemetaConstToAddress->dllfunc_addresses[id];
}

static cell AMX_NATIVE_CALL okapi_get_ptr_symbol(AMX *amx, cell *params)
{
	long address = params[1];

	char *s = get_address_symbol((void*)address);

	if (s && s[0])
	{
		g_fn_SetAmxString(amx, params[2], s, params[3]);
		return 1;
	}

	return 0;
}

long library_find_array(GameLibrary* library, long address, unsigned char* arr_or, int len)
{
	long end = (long)library->address + (long)library->length + 1 - len;

	while (address < end)
	{
		if (!memcmp((void*)address, (void*)arr_or, len))
		{
			return (long)address;
		}

		address++;
	}

	return 0;
}

bool check_sig(long address, cell* sig, int len)
{
	for (int i=0; i < len; i++)
	{
		if (sig[i] > 0xFF)
			continue;

		if ((unsigned char)sig[i] != (*(((unsigned char*)address) + i)))
		{
			return false;
		}
	}

	return true;
}

long library_find_signature(GameLibrary* library, long address, cell* sig, int len)
{
	long end = (long)library->address + (long)library->length + 1 - len;

	while (address < end)
	{
		if (check_sig(address, sig, len))
		{
			return (long)address;
		}

		address++;
	}

	return 0;
}

bool has_special(cell* array_, int pos, int len)
{
	if (pos + 5 > len)
	{
		return false;
	}

	return (array_[pos] == 0xF0) && (array_[pos + 1] == 0x90) && (array_[pos + 2] == 0x8C) && (array_[pos + 3] == 0xBB) && (array_[pos + 4] == 0x00);
}

int fix_special(cell* array_, int len)
{
	int place = 0;

	for (int i=0; i < len;)
	{
		if (has_special(array_, i, len))
		{
			array_[place++] = 0xFFFF;
			i += 5;
		}
		else
		{
			array_[place++] = array_[i++];
		}
	}

	return place;
}

static cell AMX_NATIVE_CALL okapi_ptr_get_vfunc_ptr(AMX *amx, cell *params)
{
	void* address = (void*)params[1];

	void **vtable = *((void***)(((char*)address) + G_OffsetHandler->search_virtual_table(address)));

	void* address_retrieved = vtable[params[2]];

	return (cell)address_retrieved;
}

static cell AMX_NATIVE_CALL okapi_id_to_cbase(AMX *amx, cell *params)
{
	return (cell)G_HL_TypeConversion.id_to_cbase(params[1]);
}

static cell AMX_NATIVE_CALL okapi_cbase_to_id(AMX *amx, cell *params)
{
	return (cell)G_HL_TypeConversion.cbase_to_id((void*)params[1]);
}

static cell AMX_NATIVE_CALL okapi_cbase_get_vfunc_ptr(AMX *amx, cell *params)
{
	void* cbase = G_HL_TypeConversion.id_to_cbase(params[1]);

	if (!cbase)
		return 0;

	void **vtable = *((void***)(((char*)cbase) + G_OffsetHandler->EntityVirtualTable));

	void* address = vtable[params[2]];

	return (cell)address;
}

static cell AMX_NATIVE_CALL okapi_class_get_vfunc_ptr(AMX *amx, cell *params)
{
	char* classname = g_fn_GetAmxString(amx, params[1], 0, NULL);

	/* Code from hamsandwich */
	{
		edict_t *Entity = CREATE_ENTITY();

		CALL_GAME_ENTITY(PLID, classname, &Entity->v);

		if (Entity->pvPrivateData == NULL)
		{
			REMOVE_ENTITY(Entity);

			MF_LogError(amx, AMX_ERR_NATIVE, "Failed to retrieve classtype for '%s'", classname);

			return 0;
		}

		void **vtable = *((void***)(((char*)Entity->pvPrivateData) + G_OffsetHandler->EntityVirtualTable));

		void* address = vtable[params[2]];

		REMOVE_ENTITY(Entity);

		return (cell)address;
	}
}

static cell AMX_NATIVE_CALL okapi_mod_find_sig_at(AMX *amx, cell *params)
{
	long address = params[1];
	cell* array_ = MF_GetAmxAddr(amx, params[2]);
	int len = params[3];

	len = fix_special(array_, len);

	return library_find_signature(GameLibraries.Mod, address, array_, len);
}

static cell AMX_NATIVE_CALL okapi_engine_find_sig_at(AMX *amx, cell *params)
{
	long address = params[1];
	cell* array_ = MF_GetAmxAddr(amx, params[2]);
	int len = params[3];

	len = fix_special(array_, len);

	return library_find_signature(GameLibraries.Engine, address, array_, len);
}

static cell AMX_NATIVE_CALL okapi_engine_ptr_find_array_at(AMX *amx, cell *params)
{
	long address = params[1];
	cell* array1 = MF_GetAmxAddr(amx, params[2]);

	int len = params[3];

	unsigned char* array1_char = new unsigned char[len];

	for (int i=0; i < len; i++)
	{
		array1_char[i] = (unsigned char)array1[i];
	}

	long address_res = library_find_array(GameLibraries.Engine, address, array1_char, len);

	delete[] array1_char;

	return address_res;
}

static cell AMX_NATIVE_CALL okapi_mod_ptr_find_array_at(AMX *amx, cell *params)
{
	long address = params[1];
	cell* array1 = MF_GetAmxAddr(amx, params[2]);

	int len = params[3];

	unsigned char* array1_char = new unsigned char[len];

	for (int i=0; i < len; i++)
	{
		array1_char[i] = (unsigned char)array1[i];
	}

	long address_res = library_find_array(GameLibraries.Mod, address, array1_char, len);

	delete[] array1_char;

	return address_res;
}

static cell AMX_NATIVE_CALL okapi_engine_ptr_find_byte_at(AMX *amx, cell *params)
{
	long address = params[1];

	return library_find<char>(GameLibraries.Engine, address, (char)(params[2]));
}

static cell AMX_NATIVE_CALL okapi_mod_ptr_find_byte_at(AMX *amx, cell *params)
{
	long address = params[1];

	return library_find<char>(GameLibraries.Mod, address, (char)(params[2]));
}

static cell AMX_NATIVE_CALL okapi_engine_ptr_find_float_at(AMX *amx, cell *params)
{
	long address = params[1];

	return library_find<float>(GameLibraries.Engine, address, amx_ctof(params[2]));
}

static cell AMX_NATIVE_CALL okapi_mod_ptr_find_float_at(AMX *amx, cell *params)
{
	long address = params[1];

	return library_find<float>(GameLibraries.Mod, address, amx_ctof(params[2]));
}

static cell AMX_NATIVE_CALL okapi_engine_ptr_find_int_at(AMX *amx, cell *params)
{
	long address = params[1];

	return library_find<int>(GameLibraries.Engine, address, params[2]);
}

static cell AMX_NATIVE_CALL okapi_mod_ptr_find_int_at(AMX *amx, cell *params)
{
	long address = params[1];

	return library_find<int>(GameLibraries.Mod, address, params[2]);
}

long library_find_string(GameLibrary* library, long address, char* search, int len)
{
	long end = (long)library->address + (long)library->length + 1 - (len + 1);

	while (address < end)
	{
		if (!strncmp((char*)address, search, len))
		{
			return address;
		}

		address++;
	}

	return 0;
}

static cell AMX_NATIVE_CALL okapi_mod_ptr_find_string_at(AMX *amx, cell *params)
{
	long address = params[1];

	int len;
	char* string = g_fn_GetAmxString(amx, params[2], 0, &len);

	String string_s(string);

	return library_find_string(GameLibraries.Mod, address, string_s.get_ptr(), len);
}

static cell AMX_NATIVE_CALL okapi_engine_ptr_find_string_at(AMX *amx, cell *params)
{
	long address = params[1];

	int len;
	char* string = g_fn_GetAmxString(amx, params[2], 0, &len);

	String string_s(string);

	return library_find_string(GameLibraries.Engine, address, string_s.get_ptr(), len);
}

static cell AMX_NATIVE_CALL okapi_engine_get_size(AMX *amx, cell *params)
{
	return (cell)GameLibraries.Engine->length;
}

static cell AMX_NATIVE_CALL okapi_mod_get_size(AMX *amx, cell *params)
{
	return (cell)GameLibraries.Mod->length;
}

static cell AMX_NATIVE_CALL okapi_mod_get_base_ptr(AMX *amx, cell *params)
{
	return (cell)GameLibraries.Mod->address;
}

static cell AMX_NATIVE_CALL okapi_engine_get_base_ptr(AMX *amx, cell *params)
{
	return (cell)GameLibraries.Engine->address;
}

static cell AMX_NATIVE_CALL okapi_engine_replace_float(AMX *amx, cell *params)
{
	int count = library_replace<float>(GameLibraries.Engine, amx_ctof(params[1]), amx_ctof(params[2]));

	return count;
}

static cell AMX_NATIVE_CALL okapi_mod_replace_float(AMX *amx, cell *params)
{
	int count = library_replace<float>(GameLibraries.Mod, amx_ctof(params[1]), amx_ctof(params[2]));

	return count;
}

static cell AMX_NATIVE_CALL okapi_engine_replace_int(AMX *amx, cell *params)
{
	int count = library_replace<int>(GameLibraries.Engine, params[1], params[2]);

	return count;
}

static cell AMX_NATIVE_CALL okapi_mod_replace_int(AMX *amx, cell *params)
{
	int count = library_replace<int>(GameLibraries.Mod, params[1], params[2]);

	return count;
}

int library_replace_array(GameLibrary* library, unsigned char* arr_or, unsigned char* arr_repl, int len)
{
	long start = (long)library->address;

	int count = 0;

	for (long i=0; i < library->length + 1 - len; i++)
	{
		char* address = ((char*)start) + i;

		if (!memcmp((void*)address, (void*)arr_or, len))
		{
			int protection = G_Memory.get_memory_protection((long)address);

			G_Memory.make_writable((long)address);

			memcpy(address, arr_repl, len);
			count++;

			G_Memory.set_memory_protection((long)address, protection);
		}
	}

	return count;
}

static cell AMX_NATIVE_CALL okapi_mod_replace_array(AMX *amx, cell *params)
{
	cell* array1 = MF_GetAmxAddr(amx, params[1]);
	cell* array2 = MF_GetAmxAddr(amx, params[2]);

	int len = params[3];

	unsigned char* array1_char = new unsigned char[len];
	unsigned char* array2_char = new unsigned char[len];

	for (int i=0; i < len; i++)
	{
		array1_char[i] = (unsigned char)array1[i];
		array2_char[i] = (unsigned char)array2[i];
	}

	int count = library_replace_array(GameLibraries.Mod, array1_char, array2_char, len);

	delete[] array1_char;
	delete[] array2_char;

	return count;
}

static cell AMX_NATIVE_CALL okapi_engine_replace_array(AMX *amx, cell *params)
{
	cell* array1 = MF_GetAmxAddr(amx, params[1]);
	cell* array2 = MF_GetAmxAddr(amx, params[2]);

	int len = params[3];

	unsigned char* array1_char = new unsigned char[len];
	unsigned char* array2_char = new unsigned char[len];

	for (int i=0; i < len; i++)
	{
		array1_char[i] = (unsigned char)array1[i];
		array2_char[i] = (unsigned char)array2[i];
	}

	int count = library_replace_array(GameLibraries.Engine, array1_char, array2_char, len);

	delete[] array1_char;
	delete[] array2_char;

	return count;
}

int library_replace_string(GameLibrary* library, char* str_or, char* str_repl)
{
	int len_or = strlen(str_or);

	long start = (long)library->address;

	int count = 0;

	for (long i=0; i < library->length + 1 - len_or; i++)
	{
		char* address = ((char*)start) + i;

		if (!strncmp(str_or, address, len_or))
		{
			int protection = G_Memory.get_memory_protection((long)address);

			G_Memory.make_writable((long)address);

			strcpy(address, str_repl);
			count++;

			G_Memory.set_memory_protection((long)address, protection);
		}
	}

	return count;
}

static cell AMX_NATIVE_CALL okapi_mod_replace_string(AMX *amx, cell *params)
{
	int len1;
	char* string1 = g_fn_GetAmxString(amx, params[1], 0, &len1);

	String string1_s(string1);

	int len2;
	char* string2 = g_fn_GetAmxString(amx, params[2], 0, &len2);

	String string2_s(string2);

	if (!params[3])
	{
		if (len2 > len1)
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "replacement string must be of length equal or smaller than the length of the original string");
			return 0;
		}
	}

	return library_replace_string(GameLibraries.Mod, string1_s.get_ptr(), string2_s.get_ptr());
}

static cell AMX_NATIVE_CALL okapi_engine_replace_string(AMX *amx, cell *params)
{
	int len1;
	char* string1 = g_fn_GetAmxString(amx, params[1], 0, &len1);

	String string1_s(string1);

	int len2;
	char* string2 = g_fn_GetAmxString(amx, params[2], 0, &len2);

	String string2_s(string2);

	if (!params[3])
	{
		if (len2 > len1)
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "replacement string must be of length equal or smaller than the length of the original string");
			return 0;
		}
	}

	return library_replace_string(GameLibraries.Engine, string1_s.get_ptr(), string2_s.get_ptr());
}

static cell AMX_NATIVE_CALL okapi_get_ptr_string(AMX *amx, cell *params)
{
	char* address = (char*)params[1];

	cell* string = MF_GetAmxAddr(amx, params[2]);

	int maxlen = params[3];

	int i=0;
	for (; i < maxlen; i++)
	{
		if (address[i])
		{
			string[i] = address[i];
		}
		else
			break;
	}

	string[i] = 0;

	return i;
}

static cell AMX_NATIVE_CALL okapi_set_ptr_string(AMX *amx, cell *params)
{
	char* address = (char*)params[1];

	int len;
	char* string = g_fn_GetAmxString(amx, params[2], 0, &len);

	int prot = G_Memory.get_memory_protection((long)address);

	G_Memory.make_writable((long)address);

	for (int i=0; i < len; i++)
	{
		address[i] = string[i];
	}

	address[len] = 0;

	G_Memory.set_memory_protection((long)address, prot);

	return 1;
}

static cell AMX_NATIVE_CALL okapi_set_ptr_array(AMX *amx, cell *params)
{
	unsigned char* address = (unsigned char*)params[1];
	cell* array_ = MF_GetAmxAddr(amx, params[2]);
	int count = params[3];

	int prot = G_Memory.get_memory_protection((long)address);

	G_Memory.make_writable((long)address);

	for (int i=0; i < count; i++)
	{
		address[i] = array_[i];
	}

	G_Memory.set_memory_protection((long)address, prot);

	return 1;
}

static cell AMX_NATIVE_CALL okapi_get_ptr_array(AMX *amx, cell *params)
{
	unsigned char* address = (unsigned char*)params[1];
	cell* array_ = MF_GetAmxAddr(amx, params[2]);
	int count = params[3];

	for (int i=0; i < count; i++)
	{
		array_[i] = address[i];
	}

	return 1;
}

static cell AMX_NATIVE_CALL okapi_set_ptr_byte(AMX *amx, cell *params)
{
	unsigned char* address_ptr = (unsigned char*)params[1];

	int prot = G_Memory.get_memory_protection((long)address_ptr);

	G_Memory.make_writable((long)address_ptr);

	*address_ptr = (unsigned char)params[2];

	G_Memory.set_memory_protection((long)address_ptr, prot);

	return 0;
}

static cell AMX_NATIVE_CALL okapi_get_ptr_byte(AMX *amx, cell *params)
{
	unsigned char* address_ptr = (unsigned char*)params[1];

	return *address_ptr;
}

static cell AMX_NATIVE_CALL okapi_set_mem_protect(AMX *amx, cell *params)
{
	int protection = G_Memory.convert_to_sys(params[2]);

	return G_Memory.set_memory_protection(params[1], protection);
}

static cell AMX_NATIVE_CALL okapi_get_mem_protect(AMX *amx, cell *params)
{
	int protection_sys = G_Memory.get_memory_protection(params[1]);

	int protection = G_Memory.convert_from_sys(protection_sys);

	return protection;
}

static cell AMX_NATIVE_CALL okapi_get_ptr_float(AMX *amx, cell *params)
{
	float* address_ptr = (float*)params[1];

	return amx_ftoc(*address_ptr);
}

static cell AMX_NATIVE_CALL okapi_set_ptr_float(AMX *amx, cell *params)
{
	float* address_ptr = (float*)params[1];

	int prot = G_Memory.get_memory_protection((long)address_ptr);

	G_Memory.make_writable((long)address_ptr);

	*address_ptr = amx_ctof(params[2]);

	G_Memory.set_memory_protection((long)address_ptr, prot);

	return 0;
}

static cell AMX_NATIVE_CALL okapi_set_ptr_int(AMX *amx, cell *params)
{
	int* address_ptr = (int*)params[1];

	int prot = G_Memory.get_memory_protection((long)address_ptr);

	G_Memory.make_writable((long)address_ptr);

	*address_ptr = params[2];

	G_Memory.set_memory_protection((long)address_ptr, prot);

	return 0;
}

static cell AMX_NATIVE_CALL okapi_set_ptr_ent(AMX *amx, cell *params)
{
	entvars_s** address_ptr = (entvars_s**)params[1];
	entvars_s* entvars = G_HL_TypeConversion.id_to_entvar(params[2]);

	int prot = G_Memory.get_memory_protection((long)address_ptr);

	G_Memory.make_writable((long)address_ptr);

	(*address_ptr) = entvars;

	G_Memory.set_memory_protection((long)address_ptr, prot);

	return 1;
}

static cell AMX_NATIVE_CALL okapi_get_ptr_ent(AMX *amx, cell *params)
{
	entvars_s** address_ptr = (entvars_s**)params[1];

	return G_HL_TypeConversion.entvar_to_id(*address_ptr);
}

static cell AMX_NATIVE_CALL okapi_set_ptr_cbase(AMX *amx, cell *params)
{
	void** address_ptr = (void**)params[1];
	void* cbase = G_HL_TypeConversion.id_to_cbase(params[2]);

	int prot = G_Memory.get_memory_protection((long)address_ptr);

	G_Memory.make_writable((long)address_ptr);

	(*address_ptr) = cbase;

	G_Memory.set_memory_protection((long)address_ptr, prot);

	return 1;
}

static cell AMX_NATIVE_CALL okapi_get_ptr_cbase(AMX *amx, cell *params)
{
	void** address_ptr = (void**)params[1];

	return G_HL_TypeConversion.cbase_to_id(*address_ptr);
}

static cell AMX_NATIVE_CALL okapi_set_ptr_edict(AMX *amx, cell *params)
{
	edict_s** address_ptr = (edict_s**)params[1];
	edict_s* edict = INDEXENT(params[2]);

	int prot = G_Memory.get_memory_protection((long)address_ptr);

	G_Memory.make_writable((long)address_ptr);

	(*address_ptr) = edict;

	G_Memory.set_memory_protection((long)address_ptr, prot);

	return 1;
}

static cell AMX_NATIVE_CALL okapi_get_ptr_edict(AMX *amx, cell *params)
{
	edict_s** address_ptr = (edict_s**)params[1];

	return ENTINDEX(*address_ptr);
}

static cell AMX_NATIVE_CALL okapi_get_ptr_int(AMX *amx, cell *params)
{
	int* address_ptr = (int*)params[1];

	return *address_ptr;
}

static cell AMX_NATIVE_CALL okapi_mod_get_symbol_ptr(AMX *amx, cell *params)
{
	int len;
	char* symbol = g_fn_GetAmxString(amx, params[1], 0, &len);

	GameLibrary* library = GameLibraries.Mod;

	int offset = library->find_func(symbol);

	if (!offset)
		return 0;

	return  offset + (int)library->address;
}

static cell AMX_NATIVE_CALL okapi_engine_get_symbol_ptr(AMX *amx, cell *params)
{
	int len;
	char* symbol = g_fn_GetAmxString(amx, params[1], 0, &len);

	GameLibrary* library = GameLibraries.Engine;

	int offset = library->find_func(symbol);

	if (!offset)
		return 0;

	return  offset + (int)library->address;
}

static cell AMX_NATIVE_CALL okapi_mod_get_offset_ptr(AMX *amx, cell *params)
{
	int offset = params[1];

	if ((offset >= 0) && (offset < GameLibraries.Mod->length))
	{
		return offset + (int)GameLibraries.Mod->address;
	}

	return 0;
}

static cell AMX_NATIVE_CALL okapi_engine_get_offset_ptr(AMX *amx, cell *params)
{
	int offset = params[1];

	if ((offset >= 0) && (offset < GameLibraries.Engine->length))
	{
		return offset + (int)GameLibraries.Engine->address;
	}

	return 0;
}

static cell AMX_NATIVE_CALL okapi_mod_get_ptr_offset(AMX *amx, cell *params)
{
	int offset = params[1] - (int)GameLibraries.Mod->address;

	if ((offset < 0) || (offset >= GameLibraries.Mod->length))
		offset = 0;

	return offset;
}

static cell AMX_NATIVE_CALL okapi_engine_get_ptr_offset(AMX *amx, cell *params)
{
	int offset = params[1] - (int)GameLibraries.Engine->address;

	if ((offset < 0) || (offset > GameLibraries.Engine->length))
		offset = 0;

	return offset;
}

AMX_NATIVE_INFO exports[] =
{
	{ "wl"                             , wl                              },

	{ "okapi_get_ptr_vec"              , okapi_get_ptr_vec               },

	{ "okapi_alloc"                    , okapi_alloc                     },

	{ "okapi_call_ex"                  , okapi_call_ex                   },

	{ "okapi_call"                     , okapi_call                      },

	{ "okapi_get_orig_return"          , okapi_get_orig_return           },
	{ "okapi_set_return"               , okapi_set_return                },

	{ "okapi_set_param"                , okapi_set_param                 },

	{ "okapi_current_hook"             , okapi_current_hook              },

	{ "okapi_del_hook"                 , okapi_del_hook                  },
	{ "okapi_del_current_hook"         , okapi_del_current_hook          },
	{ "okapi_add_hook"                 , okapi_add_hook                  },

	{ "okapi_build_function"           , okapi_build_function            },
	{ "okapi_build_method"             , okapi_build_method              },
	{ "okapi_build_vfunc_cbase"        , okapi_build_vfunc_cbase         },
	{ "okapi_build_vfunc_ptr"          , okapi_build_vfunc_ptr           },
	{ "okapi_build_vfunc_class"        , okapi_build_vfunc_class         },

	{ "okapi_cbase_to_id"              , okapi_cbase_to_id               },
	{ "okapi_id_to_cbase"              , okapi_id_to_cbase               },

	{ "okapi_ptr_get_vfunc_ptr"        , okapi_ptr_get_vfunc_ptr         },
	{ "okapi_class_get_vfunc_ptr"      , okapi_class_get_vfunc_ptr       },
	{ "okapi_cbase_get_vfunc_ptr"      , okapi_cbase_get_vfunc_ptr       },

	{ "okapi_get_ptr_symbol"           , okapi_get_ptr_symbol            },

	{ "okapi_mod_find_sig_at"          , okapi_mod_find_sig_at           },
	{ "okapi_engine_find_sig_at"       , okapi_engine_find_sig_at        },

	{ "okapi_mod_ptr_find_array_at"    , okapi_mod_ptr_find_array_at     },
	{ "okapi_engine_ptr_find_array_at" , okapi_engine_ptr_find_array_at  },

	{ "okapi_mod_ptr_find_byte_at"     , okapi_mod_ptr_find_byte_at      },
	{ "okapi_engine_ptr_find_byte_at"  , okapi_engine_ptr_find_byte_at   },

	{ "okapi_mod_ptr_find_float_at"    , okapi_mod_ptr_find_float_at     },
	{ "okapi_engine_ptr_find_float_at" , okapi_engine_ptr_find_float_at  },

	{ "okapi_mod_ptr_find_int_at"      , okapi_mod_ptr_find_int_at       },
	{ "okapi_engine_ptr_find_int_at"   , okapi_engine_ptr_find_int_at    },

	{ "okapi_mod_ptr_find_string_at"   , okapi_mod_ptr_find_string_at    },
	{ "okapi_engine_ptr_find_string_at", okapi_engine_ptr_find_string_at },

	{ "okapi_engine_get_size"          , okapi_engine_get_size           },
	{ "okapi_mod_get_size"             , okapi_mod_get_size              },

	{ "okapi_engine_get_base_ptr"      , okapi_engine_get_base_ptr       },
	{ "okapi_mod_get_base_ptr"         , okapi_mod_get_base_ptr          },

	{ "okapi_mod_replace_float"        , okapi_mod_replace_float         },
	{ "okapi_engine_replace_float"     , okapi_engine_replace_float      },

	{ "okapi_mod_replace_int"          , okapi_mod_replace_int           },
	{ "okapi_engine_replace_int"       , okapi_engine_replace_int        },

	{ "okapi_mod_replace_array"        , okapi_mod_replace_array         },
	{ "okapi_engine_replace_array"     , okapi_engine_replace_array      },

	{ "okapi_mod_replace_string"       , okapi_mod_replace_string        },
	{ "okapi_engine_replace_string"    , okapi_engine_replace_string     },

	{ "okapi_get_ptr_string"           , okapi_get_ptr_string            },
	{ "okapi_set_ptr_string"           , okapi_set_ptr_string            },

	{ "okapi_set_ptr_array"            , okapi_set_ptr_array             },
	{ "okapi_get_ptr_array"            , okapi_get_ptr_array             },

	{ "okapi_get_ptr_byte"             , okapi_get_ptr_byte              },
	{ "okapi_set_ptr_byte"             , okapi_set_ptr_byte              },

	{ "okapi_set_mem_protect"          , okapi_set_mem_protect           },
	{ "okapi_get_mem_protect"          , okapi_get_mem_protect           },

	{ "okapi_get_ptr_float"            , okapi_get_ptr_float             },
	{ "okapi_set_ptr_float"            , okapi_set_ptr_float             },

	{ "okapi_set_ptr_edict"            , okapi_set_ptr_edict             },
	{ "okapi_get_ptr_edict"            , okapi_get_ptr_edict             },

	{ "okapi_set_ptr_ent"              , okapi_set_ptr_ent               },
	{ "okapi_get_ptr_ent"              , okapi_get_ptr_ent               },

	{ "okapi_set_ptr_cbase"            , okapi_set_ptr_cbase             },
	{ "okapi_get_ptr_cbase"            , okapi_get_ptr_cbase             },

	{ "okapi_get_ptr_int"              , okapi_get_ptr_int               },
	{ "okapi_set_ptr_int"              , okapi_set_ptr_int               },

	{ "okapi_engine_get_ptr_offset"    , okapi_engine_get_ptr_offset     },
	{ "okapi_mod_get_ptr_offset"       , okapi_mod_get_ptr_offset        },

	{ "okapi_engine_get_offset_ptr"    , okapi_engine_get_offset_ptr     },
	{ "okapi_mod_get_offset_ptr"       , okapi_mod_get_offset_ptr        },

	{ "okapi_engine_get_symbol_ptr"    , okapi_engine_get_symbol_ptr     },
	{ "okapi_mod_get_symbol_ptr"       , okapi_mod_get_symbol_ptr        },

	{ "okapi_get_engfunc_ptr"          , okapi_get_engfunc_ptr           },
	{ "okapi_get_dllfunc_ptr"          , okapi_get_dllfunc_ptr           },

	{ "okapi_get_engfunc_ptr_by_offset", okapi_get_engfunc_ptr_by_offset },
	{ "okapi_get_dllfunc_ptr_by_offset", okapi_get_dllfunc_ptr_by_offset },

	{ NULL, NULL }
};

void OnAmxxAttach()
{
	MF_AddNatives(exports);
}

void okapi()
{
	con_printf("\n######### OKAPI #########\n\n");

	if (g_engfuncs.pfnCmd_Argc() == 1)
	{
		con_printf("okapi <command>\n");
		con_printf("okapi help <command>\n");
		con_printf("\n");

		list_commands();
	}
	else
	{
		const char* arg = g_engfuncs.pfnCmd_Argv(1);

		Command* command = Commands[(char*)arg];

		if (!command)
		{
			con_printf("invalid command\n");
		}
		else
		{
			command->exec();
		}
	}

	con_printf("\n#########################\n\n");
}

void OnMetaAttach(void)
{
	//patch_printf();
	//clean_debug_file();

	s_library* mod_library = create_library((void*)gpGamedllFuncs->dllapi_table->pfnSpawn);

	if (!mod_library)
		mod_library = create_library((void*)MDLL_Spawn);

	s_library* engine_library = create_library((void*)gpGlobals);

	GameLibraries.Engine = new GameLibraryEngine(*engine_library);
	GameLibraries.Mod = new GameLibraryMod(*mod_library);

	g_engfuncs.pfnAddServerCommand("okapi", okapi);

	Commands["help"] = new CommandHelp();

	if (!G_FakemetaConstToAddress)
		G_FakemetaConstToAddress = new FakemetaConstToAddress();

	G_ArgsTypeHandler[ArgVoid]	  = new VoidHandler();
	G_ArgsTypeHandler[ArgInt]	  = new IntHandler();
	G_ArgsTypeHandler[ArgCBase]   = new CBaseHandler();
	G_ArgsTypeHandler[ArgFloat]	  = new FloatHandler();
	G_ArgsTypeHandler[ArgVec]	  = new VecHandler();
	G_ArgsTypeHandler[ArgEntvars] = new EntvarsHandler();
	G_ArgsTypeHandler[ArgStr]	  = new StrHandler();
	G_ArgsTypeHandler[ArgEdict]	  = new EdictHandler();
	G_ArgsTypeHandler[ArgVecPtr]  = new VecPtrHandler();
}

void OnPluginsLoaded()
{
	if (!G_OffsetHandler)
	{
		G_OffsetHandler = new OffsetHandler();
	}
}

void OnPluginsUnloaded()
{
	Function::clean();

	G_Functions = Trie10<Function*>();
	G_Allocator.clear();
}