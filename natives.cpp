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


bool check_sig(long address, cell* sig, size_t sig_len)
{
	for (size_t i = 0; i < sig_len; i++)
	{
		if (sig[i] > 0xFF || sig[i] == 0x2A)
		{
			continue;
		}

		if ((unsigned char)sig[i] != (*(((unsigned char*)address) + i)))
		{
			return false;
		}
	}

	return true;
}

long do_find_signature(long start_address, long end_address, cell* sig, size_t sig_len)
{
	while (start_address < end_address)
	{
		if (check_sig(start_address, sig, sig_len))
		{
			return (long)start_address;
		}

		start_address++;
	}

	return 0;
}

long library_find_signature(GameLibrary* library, long start_address, cell* sig, size_t sig_len)
{
	long end_address = (long)library->address + (long)library->length + 1 - (long)sig_len;

	return do_find_signature(start_address, end_address, sig, sig_len);
}

long library_find_signature(long start_address, size_t range_length, cell* sig, int sig_len)
{
	long end_address = start_address + (long)range_length + 1 - (long)sig_len;

	return do_find_signature(start_address, end_address, sig, sig_len);
}

bool has_special(cell* array, int pos, int len)
{
	if (pos + 5 > len)
	{
		return false;
	}

	return (array[pos] == 0xF0) && (array[pos + 1] == 0x90) && (array[pos + 2] == 0x8C) && (array[pos + 3] == 0xBB) && (array[pos + 4] == 0x00);
}

size_t fix_special(cell* array, size_t array_len)
{
	size_t place = 0;

	for (size_t i = 0; i < array_len;)
	{
		if (has_special(array, i, array_len))
		{
			array[place++] = 0xFFFF;
			i += 5;
		}
		else
		{
			array[place++] = array[i++];
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

static cell AMX_NATIVE_CALL okapi_find_sig(AMX *amx, cell *params)
{
	long start_address  = params[1];
	size_t range_length = params[2];

	cell* array = MF_GetAmxAddr(amx, params[3]);
	size_t array_len = params[4];

	array_len = fix_special(array, array_len);

	return library_find_signature(start_address, range_length, array, array_len);
}

static cell AMX_NATIVE_CALL okapi_get_library_size(AMX *amx, cell *params)
{
	return (cell)G_GameLibraries.Mod->length;

	int length;
	const char *libName = MF_GetAmxString(amx, params[1], 0, &length);

	GameLibrary* lib;

	if (G_GameLibraries.Others.retrieve(libName, &lib))
	{
		return lib->length;
	}

	return 0;
}

static cell AMX_NATIVE_CALL okapi_get_library_ptr(AMX *amx, cell *params)
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

long do_find_array(long start_address, long end_address, unsigned char* arr_or, size_t arr_len)
{
	while (start_address < end_address)
	{
		if (!memcmp((void*)start_address, (void*)arr_or, arr_len))
		{
			return (long)start_address;
		}

		start_address++;
	}

	return 0;
}

long library_find_array(GameLibrary* library, long start_address, unsigned char* arr_or, size_t arr_len)
{
	long end_address = (long)library->address + (long)library->length + 1 - (long)arr_len;

	return do_find_array(start_address, end_address, arr_or, arr_len);
}

long library_find_array(long start_address, size_t range_length, unsigned char* arr_or, size_t arr_len)
{
	long end_address = (long)start_address + (long)range_length + 1 - (long)arr_len;

	return do_find_array(start_address, end_address, arr_or, arr_len);
}

long do_find_string(long start_address, long end_address, const char* search, size_t search_length)
{
	while (start_address < end_address)
	{
		if (!strncmp((char*)start_address, search, search_length))
		{
			return start_address;
		}

		start_address++;
	}

	return 0;
}

long library_find_string(GameLibrary* library, long start_address, const char* search, size_t search_length)
{
	long end_address = (long)library->address + (long)library->length + 1 - ((long)search_length + 1);

	return do_find_string(start_address, end_address, search, search_length);
}

long library_find_string(long start_address, size_t range_length, const char* search, size_t search_length)
{
	long end_address = (long)start_address + (long)range_length + 1 - ((long)search_length + 1);

	return do_find_string(start_address, end_address, search, search_length);
}

static cell AMX_NATIVE_CALL okapi_mem_find(AMX *amx, cell *params)
{
	int type = params[3];

	if (type != MemType_String && type != MemType_Int && type != MemType_Foat && type != MemType_Array)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid data type");
		return 0;
	}

	size_t argumentsCount = *params / sizeof(cell) - 3;

	if (argumentsCount < 2 || argumentsCount > 3)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Expected 2 or 3 parameters, got %d", argumentsCount);
		return 0;
	}
	else if (type == MemType_Array && argumentsCount == 1)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Expected an array size parameter");
		return 0;
	}

	cell start_address   = params[1];
	size_t range_length  = params[2];
	cell value_to_search = params[4];

	switch (type)
	{
		case MemType_Byte:
		{
			return library_find<char>(start_address, range_length, value_to_search);
		}
		case MemType_Int:
		{
			return library_find<int>(start_address, range_length, value_to_search);
		}
		case MemType_Foat:
		{
			return library_find<float>(start_address, range_length, amx_ctof(value_to_search));
		}
		case MemType_Array:
		{
			cell* array = MF_GetAmxAddr(amx, value_to_search);

			size_t length = params[5];

			unsigned char* array_char = new unsigned char[length];

			for (size_t i = 0; i < length; i++)
			{
				array_char[i] = (unsigned char)array[i];
			}

			delete[] array_char;

			return library_find_array(start_address, range_length, array_char, length);
		}
		case MemType_String:
		{
			int length;
			const char* string = MF_GetAmxString(amx, value_to_search, 0, &length);

			return library_find_string(start_address, range_length, string, (size_t)length);
		}
	}

	return 0;
}

int do_replace_array(long start, int length, unsigned char* arr_or, unsigned char* arr_repl, size_t arr_len)
{
	int count = 0;

	for (long i = 0; i < length + 1 - (long)arr_len; i++)
	{
		char* address = ((char*)start) + i;

		if (!memcmp((void*)address, (void*)arr_or, arr_len))
		{
			int protection = G_Memory.get_memory_protection((long)address);

			G_Memory.make_writable((long)address);

			memcpy(address, arr_repl, arr_len);
			count++;

			G_Memory.set_memory_protection((long)address, protection);
		}
	}

	return count;
}

int library_replace_array(GameLibrary* library, unsigned char* arr_or, unsigned char* arr_repl, size_t arr_len)
{
	return do_replace_array((long)library->address, library->length, arr_or, arr_repl, arr_len);
}

int library_replace_array(cell start_address, int length, unsigned char* arr_or, unsigned char* arr_repl, size_t arr_len)
{
	return do_replace_array((long)start_address, length, arr_or, arr_repl, arr_len);
}

int do_replace_string(long start, int length, const char* str_or, const char* str_repl)
{
	int count = 0;
	size_t len_or = strlen(str_or);

	for (long i = 0; i < length + 1 - (long)len_or; i++)
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

int library_replace_string(GameLibrary* library, const char* str_or, const char* str_repl)
{
	return do_replace_string((long)library->address, library->length, str_or, str_repl);
}

int library_replace_string(cell start_address, int length, const char* str_or, const char* str_repl)
{
	return do_replace_string((long)start_address, length, str_or, str_repl);
}

static cell AMX_NATIVE_CALL okapi_mem_replace(AMX *amx, cell *params)
{
	int type = params[3];

	if (type != MemType_String && type != MemType_Int && type != MemType_Foat && type != MemType_Array)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid data type");
		return 0;
	}

	size_t argumentsCount = *params / sizeof(cell) - 3;

	if (argumentsCount < 2 || argumentsCount > 3)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Expected 2 or 3 parameters, got %d", argumentsCount);
		return 0;
	}
	else if (type == MemType_Array && argumentsCount == 1)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Expected an array size parameter");
		return 0;
	}

	cell start_address   = params[1];
	size_t search_length = params[2];

	cell old_value = params[4];
	cell new_value = params[5];

	int count = 0;

	switch (type)
	{
		case MemType_Int:
		{
			count = library_replace<int>(start_address, search_length, old_value, new_value);
			break;
		}
		case MemType_Foat:
		{
			count = library_replace<float>(start_address, search_length, amx_ctof(old_value), amx_ctof(new_value));
			break;
		}
		case MemType_Array:
		{
			cell* old_array = MF_GetAmxAddr(amx, old_value);
			cell* new_array = MF_GetAmxAddr(amx, new_value);

			size_t length = params[6];

			unsigned char* old_array_char = new unsigned char[length];
			unsigned char* new_array_char = new unsigned char[length];

			for (size_t i = 0; i < length; ++i)
			{
				old_array_char[i] = (unsigned char)old_array[i];
				new_array_char[i] = (unsigned char)new_array[i];
			}

			int count = library_replace_array(start_address, search_length, old_array_char, new_array_char, length);

			delete[] old_array_char;
			delete[] new_array_char;

			break;
		}
		case MemType_String:
		{
			int old_length, new_length;
			const char* old_stirng = MF_GetAmxString(amx, old_value, 0, &old_length);
			const char* new_string = MF_GetAmxString(amx, new_value, 1, &new_length);

			if ((argumentsCount == 3 && !params[5]))
			{
				if (new_length > old_length)
				{
					MF_LogError(amx, AMX_ERR_NATIVE, "Replacement string must be of length equal or smaller than the length of the original string");
					return 0;
				}
			}

			count = library_replace_string(start_address, search_length, old_stirng, new_string);

			break;
		}
	}

	return count;
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

static cell AMX_NATIVE_CALL okapi_find_symbol(AMX *amx, cell *params)
{
	int length;
	void*  lib_address = (void*)params[1];
	const char* symbol = MF_GetAmxString(amx, params[2], 0, &length);

	GameLibrary* library = get_lib_from_address(lib_address);

	if (library)
	{
		int offset = library->find_func(symbol);

		if (offset != 0)
		{
			return  (int)library->address + offset;
		}
	}

	return 0;
}

static cell AMX_NATIVE_CALL okapi_find_library_by_ptr(AMX *amx, cell *params)
{
	long address = (long)params[1];

	if (address != 0)
	{
		StringHashMap<GameLibrary*>::iterator iter = G_GameLibraries.Others.iter();

		while (!iter.empty())
		{
			if (iter->value->contains_address(address))
			{
				*MF_GetAmxAddr(amx, params[2]) = (long)iter->value->address;
				*MF_GetAmxAddr(amx, params[3]) = iter->value->length;

				MF_SetAmxString(amx, params[4], iter->value->get_name(), params[5]);

				return 1;
			}

			iter.next(); 
		}
	}

	return 0;
}

AMX_NATIVE_INFO OkapiNatives[] =
{
	{ "wl"                             , wl },

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

	{ "okapi_find_symbol"              , okapi_find_symbol },
	{ "okapi_find_sig"                 , okapi_find_sig },

	{ "okapi_get_library_size"         , okapi_get_library_size },
	{ "okapi_get_library_ptr"          , okapi_get_library_ptr },
	{ "okapi_find_library_by_ptr"      , okapi_find_library_by_ptr },

	{ "okapi_mem_find"                 , okapi_mem_find },
	{ "okapi_mem_replace"              , okapi_mem_replace },
	{ "okapi_mem_get"                  , okapi_mem_get },
	{ "okapi_mem_set"                  , okapi_mem_set },

	{ "okapi_set_mem_protect"          , okapi_set_mem_protect },
	{ "okapi_get_mem_protect"          , okapi_get_mem_protect },

	{ "okapi_get_engfunc_ptr"          , okapi_get_engfunc_ptr },
	{ "okapi_get_dllfunc_ptr"          , okapi_get_dllfunc_ptr },

	{ "okapi_get_engfunc_ptr_by_offset", okapi_get_engfunc_ptr_by_offset },
	{ "okapi_get_dllfunc_ptr_by_offset", okapi_get_dllfunc_ptr_by_offset },

	{ NULL, NULL }
};
