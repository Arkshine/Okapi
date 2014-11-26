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

#include "natives.h"

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
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter count. Expecting %d parameters", expected_params_n);

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
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter count. Expecting %d parameters", expected_params_n);

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
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter count. Expecting %d parameters", expected_params_n);

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

	const char* callback = MF_GetAmxString(amx, params[2], 0, NULL);

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

	FunctionsDataMap::Insert insert = G_Functions.findForAdd(address);

	if (insert.found())
	{
		return (cell)insert->value;
	}

	if (!G_Functions.add(insert))
	{
		return 0;
	}

	TypeHandler* return_handler = G_ArgsTypeHandler[params[2]];
	ke::Vector<TypeHandler*>* arguments_handlers = new ke::Vector < TypeHandler* > ;

	arguments_handlers->append(G_ArgsTypeHandler[params[3]]);

	for (int i=4; i <= params_n; i++)
	{
		int id = *MF_GetAmxAddr(amx, params[i]);
		arguments_handlers->append(G_ArgsTypeHandler[id]);
	}

	Function* function = new FunctionMethod((void*)address, arguments_handlers, return_handler);

	insert->key = address;
	insert->value = function;

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

	FunctionsDataMap::Insert insert = G_Functions.findForAdd((long)entry_address);

	if (insert.found())
	{
		return (cell)insert->value;
	}

	if (!G_Functions.add(insert))
	{
		return 0;
	}

	TypeHandler* return_handler = G_ArgsTypeHandler[params[3]];
	ke::Vector<TypeHandler*>* arguments_handlers = new ke::Vector < TypeHandler* > ;

	arguments_handlers->append(G_ArgsTypeHandler[ArgInt]);

	for (int i=4; i <= params_n; i++)
	{
		int id = *MF_GetAmxAddr(amx, params[i]);
		arguments_handlers->append(G_ArgsTypeHandler[id]);
	}

	Function* function = new FunctionVirtual((void*)entry_address, arguments_handlers, return_handler);

	insert->key = (long)entry_address;
	insert->value = function;

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

	FunctionsDataMap::Insert insert = G_Functions.findForAdd((long)entry_address);

	if (insert.found())
	{
		return (cell)insert->value;
	}

	if (!G_Functions.add(insert))
	{
		return 0;
	}

	TypeHandler* return_handler = G_ArgsTypeHandler[params[3]];
	ke::Vector<TypeHandler*>* arguments_handlers = new ke::Vector < TypeHandler* > ;

	arguments_handlers->append(G_ArgsTypeHandler[ArgCBase]);

	for (int i=4; i <= params_n; i++)
	{
		int id = *MF_GetAmxAddr(amx, params[i]);
		arguments_handlers->append(G_ArgsTypeHandler[id]);
	}

	Function* function = new FunctionVirtual((void*)entry_address, arguments_handlers, return_handler);

	insert->key = (long)entry_address;
	insert->value = function;

	return (cell)function;
}

static cell AMX_NATIVE_CALL okapi_build_vfunc_class(AMX *amx, cell *params)
{
	int params_n = params[0] / sizeof(cell);

	const char* classname = MF_GetAmxString(amx, params[1], 0, NULL);
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

	FunctionsDataMap::Insert insert = G_Functions.findForAdd((long)entry_address);

	if (insert.found())
	{
		return (cell)insert->value;
	}

	if (!G_Functions.add(insert))
	{
		return 0;
	}

	TypeHandler* return_handler = G_ArgsTypeHandler[params[3]];
	ke::Vector<TypeHandler*>* arguments_handlers = new ke::Vector < TypeHandler* > ;

	arguments_handlers->append(G_ArgsTypeHandler[ArgCBase]);

	for (int i=4; i <= params_n; i++)
	{
		int id = *MF_GetAmxAddr(amx, params[i]);
		arguments_handlers->append(G_ArgsTypeHandler[id]);
	}

	Function* function = new FunctionVirtual((void*)entry_address, arguments_handlers, return_handler);

	insert->key = (long)entry_address;
	insert->value = function;

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

	FunctionsDataMap::Insert insert = G_Functions.findForAdd(address);

	if (insert.found())
	{
		return (cell)insert->value;
	}

	if (!G_Functions.add(insert))
	{
		return 0;
	}

	TypeHandler* return_handler = G_ArgsTypeHandler[params[2]];
	ke::Vector<TypeHandler*>* arguments_handlers = new ke::Vector < TypeHandler* > ;

	for (int i=3; i <= params_n; i++)
	{
		int id = *MF_GetAmxAddr(amx, params[i]);
		arguments_handlers->append(G_ArgsTypeHandler[id]);
	}

	Function* function = new FunctionCdecl((void*)address, arguments_handlers, return_handler);

	insert->key = address;
	insert->value = function;

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

	if ((params[1] < 0) || (params[1] >= EngFunc_Count))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid engfunc id");

		return 0;
	}

	return (cell)G_FakemetaConstToAddress->engfunc_addresses[id];
}

static cell AMX_NATIVE_CALL okapi_get_dllfunc_ptr(AMX *amx, cell *params)
{
	int id = params[1];

	if ((params[1] < 0) || (params[1] >= DLLFunc_Count))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid dllfunc id");

		return 0;
	}

	return (cell)G_FakemetaConstToAddress->dllfunc_addresses[id];
}

static cell AMX_NATIVE_CALL okapi_get_ptr_symbol(AMX *amx, cell *params)
{
	long address = params[1];

	const char *s = get_address_symbol((void*)address);

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
	const char* classname = MF_GetAmxString(amx, params[1], 0, NULL);

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

GameLibrary* get_lib_from_address(void *address)
{
	StringHashMap<GameLibrary*>::iterator iter = G_GameLibraries.Others.iter();

	while (!iter.empty())
	{
		if (iter->value->address == address)
		{
			return iter->value;
		}

		iter.next();
	}
		
	return NULL;
}

static cell AMX_NATIVE_CALL okapi_find_sig_at(AMX *amx, cell *params)
{
	long lib_address   = params[1];
	long start_address = params[2];

	cell* array_ = MF_GetAmxAddr(amx, params[3]);
	int len = params[4];

	len = fix_special(array_, len);

	return library_find_signature(get_lib_from_address((void *)lib_address), start_address, array_, len);
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

	long address_res = library_find_array(G_GameLibraries.Engine, address, array1_char, len);

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

	long address_res = library_find_array(G_GameLibraries.Mod, address, array1_char, len);

	delete[] array1_char;

	return address_res;
}

static cell AMX_NATIVE_CALL okapi_engine_ptr_find_byte_at(AMX *amx, cell *params)
{
	long address = params[1];

	return library_find<char>(G_GameLibraries.Engine, address, (char)(params[2]));
}

static cell AMX_NATIVE_CALL okapi_mod_ptr_find_byte_at(AMX *amx, cell *params)
{
	long address = params[1];

	return library_find<char>(G_GameLibraries.Mod, address, (char)(params[2]));
}

static cell AMX_NATIVE_CALL okapi_engine_ptr_find_float_at(AMX *amx, cell *params)
{
	long address = params[1];

	return library_find<float>(G_GameLibraries.Engine, address, amx_ctof(params[2]));
}

static cell AMX_NATIVE_CALL okapi_mod_ptr_find_float_at(AMX *amx, cell *params)
{
	long address = params[1];

	return library_find<float>(G_GameLibraries.Mod, address, amx_ctof(params[2]));
}

static cell AMX_NATIVE_CALL okapi_engine_ptr_find_int_at(AMX *amx, cell *params)
{
	long address = params[1];

	return library_find<int>(G_GameLibraries.Engine, address, params[2]);
}

static cell AMX_NATIVE_CALL okapi_mod_ptr_find_int_at(AMX *amx, cell *params)
{
	long address = params[1];

	return library_find<int>(G_GameLibraries.Mod, address, params[2]);
}

long library_find_string(GameLibrary* library, long address, const char* search, int len)
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
	const char* string = MF_GetAmxString(amx, params[2], 0, &len);

	return library_find_string(G_GameLibraries.Mod, address, string, len);
}

static cell AMX_NATIVE_CALL okapi_engine_ptr_find_string_at(AMX *amx, cell *params)
{
	long address = params[1];

	int len;
	const char* string = MF_GetAmxString(amx, params[2], 0, &len);

	return library_find_string(G_GameLibraries.Engine, address, string, len);
}

static cell AMX_NATIVE_CALL okapi_engine_get_size(AMX *amx, cell *params)
{
	return (cell)G_GameLibraries.Engine->length;
}

static cell AMX_NATIVE_CALL okapi_mod_get_size(AMX *amx, cell *params)
{
	return (cell)G_GameLibraries.Mod->length;
}

static cell AMX_NATIVE_CALL okapi_get_base_ptr(AMX *amx, cell *params)
{
	int length;
	const char *libName = MF_GetAmxString(amx, params[1], 0, &length);

	GameLibrary* lib;

	if (G_GameLibraries.Others.retrieve(libName, &lib))
	{
		return (cell)lib->address;
	}

	return 0;
}

static cell AMX_NATIVE_CALL okapi_engine_replace_float(AMX *amx, cell *params)
{
	int count = library_replace<float>(G_GameLibraries.Engine, amx_ctof(params[1]), amx_ctof(params[2]));

	return count;
}

static cell AMX_NATIVE_CALL okapi_mod_replace_float(AMX *amx, cell *params)
{
	int count = library_replace<float>(G_GameLibraries.Mod, amx_ctof(params[1]), amx_ctof(params[2]));

	return count;
}

static cell AMX_NATIVE_CALL okapi_engine_replace_int(AMX *amx, cell *params)
{
	int count = library_replace<int>(G_GameLibraries.Engine, params[1], params[2]);

	return count;
}

static cell AMX_NATIVE_CALL okapi_mod_replace_int(AMX *amx, cell *params)
{
	int count = library_replace<int>(G_GameLibraries.Mod, params[1], params[2]);

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

	int count = library_replace_array(G_GameLibraries.Mod, array1_char, array2_char, len);

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

	int count = library_replace_array(G_GameLibraries.Engine, array1_char, array2_char, len);

	delete[] array1_char;
	delete[] array2_char;

	return count;
}

int library_replace_string(GameLibrary* library, const char* str_or, const char* str_repl)
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
	int len1, len2;
	const char* string1 = MF_GetAmxString(amx, params[1], 0, &len1);
	const char* string2 = MF_GetAmxString(amx, params[2], 1, &len2);


	if (!params[3])
	{
		if (len2 > len1)
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "replacement string must be of length equal or smaller than the length of the original string");
			return 0;
		}
	}

	return library_replace_string(G_GameLibraries.Mod, string1, string2);
}

static cell AMX_NATIVE_CALL okapi_engine_replace_string(AMX *amx, cell *params)
{
	int len1, len2;
	const char* string1 = MF_GetAmxString(amx, params[1], 0, &len1);
	const char* string2 = MF_GetAmxString(amx, params[2], 1, &len2);

	if (!params[3])
	{
		if (len2 > len1)
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "replacement string must be of length equal or smaller than the length of the original string");
			return 0;
		}
	}

	return library_replace_string(G_GameLibraries.Engine, string1, string2);
}

static cell AMX_NATIVE_CALL okapi_mem_get(AMX *amx, cell *params)
{
	int memory_type = params[2];

	if (memory_type < 0 || memory_type > MemType_Count)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid memory type");
		return 0;
	}

	size_t argumentsCount = *params / sizeof(cell) - 2;

	if (argumentsCount > 2)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Expected 2 parameters max, got %d", argumentsCount);
		return 0;
	}

	enum
	{
		Ret_Int,
		Ret_Float,
		Ret_Vec,
		Ret_String,
		Ret_Array,
	};

	union
	{
		int      i;
		float    f;
		byte     b;

	} returns;

	int valueType;
	Vector v;
	unsigned char* a;
	const cell* s;

	cell address = params[1];
	cell value   = params[3];

	switch (memory_type)
	{
		case MemType_Int:
		{
			returns.i = *(int*)address;
			valueType = Ret_Int;
			break;
		}
		case MemType_Byte:
		{
			returns.b = *(unsigned char*)address;
			valueType = Ret_Int;
			break;
		}
		case MemType_Foat:
		{
			returns.f = *(float*)address;
			valueType = Ret_Float;
			break;
		}
		case MemType_Edict:
		{
			returns.i = ENTINDEX(*(edict_t **)address);
			valueType = Ret_Int;
			break;
		}
		case MemType_Entvars:
		{
			returns.i = G_HL_TypeConversion.entvar_to_id(*(entvars_s**)address);
			valueType = Ret_Int;
			break;
		}
		case MemType_Cbase:
		{
			returns.i = G_HL_TypeConversion.cbase_to_id(*(void**)address);
			valueType = Ret_Int;
			break;
		}
		case MemType_Vector:
		{
			v = *(Vector*)address;
			valueType = Ret_Vec;
			break;
		}
		case MemType_Array:
		{
			a = (unsigned char*)address;
			valueType = Ret_Array;
			break;
		}
		case MemType_String:
		{
			s = (cell*)address;
			valueType = Ret_String;
			break;
		}
	}

	switch (argumentsCount)
	{
		case 0: 
		{
			switch (valueType)
			{
				case Ret_Int:
				{
					return returns.i;
				}
				case Ret_Float:
				{
					return amx_ftoc(returns.f);
				}
				case Ret_Array:
				{
					return (cell)a[0];
				}
				default:
				{
					MF_LogError(amx, AMX_ERR_NATIVE, "Invalid return type");
					return 0;
				}
			}
		}
		case 1:
		{
			cell *addr = MF_GetAmxAddr(amx, value);

			switch (valueType)
			{
				case Ret_Int:
				case Ret_Float:
				{
					*addr = amx_ftoc(returns.f);
				}
				case Ret_Vec:
				{
					addr[0] = amx_ftoc(v.x);
					addr[1] = amx_ftoc(v.y);
					addr[2] = amx_ftoc(v.z);
				}
				default:
				{
					MF_LogError(amx, AMX_ERR_NATIVE, "Invalid return type");
					return 0;
				}
			}

			return 1;
		}
		case 2:
		{
			cell size = *(MF_GetAmxAddr(amx, params[4]));

			switch (valueType)
			{
				case Ret_String:
				{
					return MF_SetAmxStringUTF8Char(amx, value, (const char*)s, strlen((const char*)s), size);
				}
				case Ret_Int:
				{
					char buffer[32];
					UTIL_Format(buffer, sizeof(buffer) - 1, "%d", returns.i);

					return MF_SetAmxString(amx, value, buffer, size);
				}
				case Ret_Float:
				{
					char buffer[32];
					UTIL_Format(buffer, sizeof(buffer) - 1, "%f", returns.f);

					return MF_SetAmxString(amx, value, buffer, size);
				}
				case Ret_Vec:
				{
					char buffer[32];
					UTIL_Format(buffer, sizeof(buffer) - 1, "%f %f %f", v.x, v.y, v.z);

					return MF_SetAmxString(amx, value, buffer, size);
				}
				case Ret_Array:
				{
					cell* addr = MF_GetAmxAddr(amx, value);

					for (int i = 0; i < size; ++i)
					{
						addr[i] = a[i];
					}
				}
				default:
				{
					MF_LogError(amx, AMX_ERR_NATIVE, "Invalid return type");
					return 0;
				}
			}

			return 1;
		}
	}

	return 0;
}

static cell AMX_NATIVE_CALL okapi_mem_set(AMX *amx, cell *params)
{
	int memory_type = params[2];

	if (memory_type < 0 || memory_type > MemType_Count)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid memory type");
		return 0;
	}

	size_t argumentsCount = *params / sizeof(cell) - 2;

	if (!argumentsCount || argumentsCount > 2)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Expected 1 or 2 parameters, got %d", argumentsCount);
		return 0;
	}
	else if (memory_type == MemType_Array && argumentsCount == 1)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Expected an array size parameter");
		return 0;
	}

	cell address = params[1];
	cell value   = params[3];

	int prot = G_Memory.get_memory_protection((long)address);

	G_Memory.make_writable((long)address);

	switch (memory_type)
	{
		case MemType_Int:
		{
			*(int*)address = value;
			break;
		}
		case MemType_Byte:
		{
			*(unsigned char*)address = (unsigned char)value;
			break;
		}
		case MemType_Foat:
		{
			*(float*)address = amx_ctof(value);
			break;
		}
		case MemType_Edict:
		{
			*(edict_s**)address = INDEXENT(value);
			break;
		}
		case MemType_Entvars:
		{
			*(entvars_s**)address = G_HL_TypeConversion.id_to_entvar(value);
			break;
		}
		case MemType_Cbase:
		{
			*(void**)address = G_HL_TypeConversion.id_to_cbase(value);
			break;
		}
		case MemType_Vector:
		{
			Vector* address_ = (Vector*)address;
			cell* vec = MF_GetAmxAddr(amx, value);

			(*address_).x = amx_ctof(vec[0]);
			(*address_).y = amx_ctof(vec[1]);
			(*address_).z = amx_ctof(vec[2]);
			break;
		}
		case MemType_Array:
		{
			unsigned char* address_ = (unsigned char*)address;
			cell* array = MF_GetAmxAddr(amx, value);
			int size = params[4];

			for (int i = 0; i < size; ++i)
			{
				address_[i] = array[i];
			}
			break;
		}
		case MemType_String:
		{
			int length;
			unsigned char* address_ = (unsigned char*)address;
			const char* string = MF_GetAmxString(amx, value, 0, &length);

			for (int i = 0; i < length; i++)
			{
				address_[i] = string[i];
			}

			address_[length] = '\0';
			break;
		}
	}

	G_Memory.set_memory_protection((long)address, prot);

	return 1;
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

static cell AMX_NATIVE_CALL okapi_mod_get_symbol_ptr(AMX *amx, cell *params)
{
	int len;
	const char* symbol = MF_GetAmxString(amx, params[1], 0, &len);

	GameLibrary* library = G_GameLibraries.Mod;

	int offset = library->find_func(symbol);

	if (!offset)
		return 0;

	return  offset + (int)library->address;
}

static cell AMX_NATIVE_CALL okapi_engine_get_symbol_ptr(AMX *amx, cell *params)
{
	int len;
	const char* symbol = MF_GetAmxString(amx, params[1], 0, &len);

	GameLibrary* library = G_GameLibraries.Engine;

	int offset = library->find_func(symbol);

	if (!offset)
		return 0;

	return  offset + (int)library->address;
}

static cell AMX_NATIVE_CALL okapi_mod_get_offset_ptr(AMX *amx, cell *params)
{
	int offset = params[1];

	if ((offset >= 0) && (offset < G_GameLibraries.Mod->length))
	{
		return offset + (int)G_GameLibraries.Mod->address;
	}

	return 0;
}

static cell AMX_NATIVE_CALL okapi_engine_get_offset_ptr(AMX *amx, cell *params)
{
	int offset = params[1];

	if ((offset >= 0) && (offset < G_GameLibraries.Engine->length))
	{
		return offset + (int)G_GameLibraries.Engine->address;
	}

	return 0;
}

static cell AMX_NATIVE_CALL okapi_mod_get_ptr_offset(AMX *amx, cell *params)
{
	int offset = params[1] - (int)G_GameLibraries.Mod->address;

	if ((offset < 0) || (offset >= G_GameLibraries.Mod->length))
		offset = 0;

	return offset;
}

static cell AMX_NATIVE_CALL okapi_engine_get_ptr_offset(AMX *amx, cell *params)
{
	int offset = params[1] - (int)G_GameLibraries.Engine->address;

	if ((offset < 0) || (offset > G_GameLibraries.Engine->length))
		offset = 0;

	return offset;
}

AMX_NATIVE_INFO OkapiNatives[] =
{
	{ "wl"                             , wl },

	{ "okapi_get_ptr_vec"              , okapi_get_ptr_vec },

	{ "okapi_alloc"                    , okapi_alloc },

	{ "okapi_call_ex"                  , okapi_call_ex },

	{ "okapi_call"                     , okapi_call },

	{ "okapi_get_orig_return"          , okapi_get_orig_return },
	{ "okapi_set_return"               , okapi_set_return },

	{ "okapi_set_param"                , okapi_set_param },

	{ "okapi_current_hook"             , okapi_current_hook },

	{ "okapi_del_hook"                 , okapi_del_hook },
	{ "okapi_del_current_hook"         , okapi_del_current_hook },
	{ "okapi_add_hook"                 , okapi_add_hook },

	{ "okapi_build_function"           , okapi_build_function },
	{ "okapi_build_method"             , okapi_build_method },
	{ "okapi_build_vfunc_cbase"        , okapi_build_vfunc_cbase },
	{ "okapi_build_vfunc_ptr"          , okapi_build_vfunc_ptr },
	{ "okapi_build_vfunc_class"        , okapi_build_vfunc_class },

	{ "okapi_cbase_to_id"              , okapi_cbase_to_id },
	{ "okapi_id_to_cbase"              , okapi_id_to_cbase },

	{ "okapi_ptr_get_vfunc_ptr"        , okapi_ptr_get_vfunc_ptr },
	{ "okapi_class_get_vfunc_ptr"      , okapi_class_get_vfunc_ptr },
	{ "okapi_cbase_get_vfunc_ptr"      , okapi_cbase_get_vfunc_ptr },

	{ "okapi_get_ptr_symbol"           , okapi_get_ptr_symbol },

	{ "okapi_find_sig_at"              , okapi_find_sig_at },

	{ "okapi_mod_ptr_find_array_at"    , okapi_mod_ptr_find_array_at },
	{ "okapi_engine_ptr_find_array_at" , okapi_engine_ptr_find_array_at },

	{ "okapi_mod_ptr_find_byte_at"     , okapi_mod_ptr_find_byte_at },
	{ "okapi_engine_ptr_find_byte_at"  , okapi_engine_ptr_find_byte_at },

	{ "okapi_mod_ptr_find_float_at"    , okapi_mod_ptr_find_float_at },
	{ "okapi_engine_ptr_find_float_at" , okapi_engine_ptr_find_float_at },

	{ "okapi_mod_ptr_find_int_at"      , okapi_mod_ptr_find_int_at },
	{ "okapi_engine_ptr_find_int_at"   , okapi_engine_ptr_find_int_at },

	{ "okapi_mod_ptr_find_string_at"   , okapi_mod_ptr_find_string_at },
	{ "okapi_engine_ptr_find_string_at", okapi_engine_ptr_find_string_at },

	{ "okapi_engine_get_size"          , okapi_engine_get_size },
	{ "okapi_mod_get_size"             , okapi_mod_get_size },

	{ "okapi_get_base_ptr"             , okapi_get_base_ptr },

	{ "okapi_mod_replace_float"        , okapi_mod_replace_float },
	{ "okapi_engine_replace_float"     , okapi_engine_replace_float },

	{ "okapi_mod_replace_int"          , okapi_mod_replace_int },
	{ "okapi_engine_replace_int"       , okapi_engine_replace_int },

	{ "okapi_mod_replace_array"        , okapi_mod_replace_array },
	{ "okapi_engine_replace_array"     , okapi_engine_replace_array },

	{ "okapi_mod_replace_string"       , okapi_mod_replace_string },
	{ "okapi_engine_replace_string"    , okapi_engine_replace_string },

	{ "okapi_mem_get"                  , okapi_mem_get },
	{ "okapi_mem_set"                  , okapi_mem_set },

	{ "okapi_set_mem_protect"          , okapi_set_mem_protect },
	{ "okapi_get_mem_protect"          , okapi_get_mem_protect },

	{ "okapi_engine_get_ptr_offset"    , okapi_engine_get_ptr_offset },
	{ "okapi_mod_get_ptr_offset"       , okapi_mod_get_ptr_offset },

	{ "okapi_engine_get_offset_ptr"    , okapi_engine_get_offset_ptr },
	{ "okapi_mod_get_offset_ptr"       , okapi_mod_get_offset_ptr },

	{ "okapi_engine_get_symbol_ptr"    , okapi_engine_get_symbol_ptr },
	{ "okapi_mod_get_symbol_ptr"       , okapi_mod_get_symbol_ptr },

	{ "okapi_get_engfunc_ptr"          , okapi_get_engfunc_ptr },
	{ "okapi_get_dllfunc_ptr"          , okapi_get_dllfunc_ptr },

	{ "okapi_get_engfunc_ptr_by_offset", okapi_get_engfunc_ptr_by_offset },
	{ "okapi_get_dllfunc_ptr_by_offset", okapi_get_dllfunc_ptr_by_offset },

	{ NULL, NULL }
};
