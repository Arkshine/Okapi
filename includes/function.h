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

#ifndef FUNCTION_H
#define FUNCTION_H

#include "amxxmodule.h"
#include <am-vector.h>
#include "type_handler.h"
#include "allocator.h"
#include "memory_.h"
#include "assembly_create.h"

enum OkapiRet
{
	OkapiRetIgnore,
	OkapiRetOverride,
	OkapiRetSupercede
};

class Function;

struct AMX_Hook
{
	Function* function;
	int amx_hook;
	int phase;

	AMX_Hook(Function* function, int amx_hook, int phase) : function(function), amx_hook(amx_hook), phase(phase){}

	~AMX_Hook()
	{
		MF_UnregisterSPForward(amx_hook);
	}
};

class Function
{
		static ke::Vector<Function*> functions;
		static ke::Vector<AMX_Hook*> hooks_stack;

		static long Gate(void* ebp, void* eip, Function* func, void** stack, ...);
		static float GateFloat(void* ebp, void* eip, Function* func, void** stack, ...);

	protected:

		void* address;
		unsigned char* trampoline;
		ke::AutoPtr< ke::Vector<TypeHandler*> > arguments_handlers;
		TypeHandler* return_handler;

		ke::Vector<AMX_Hook*> amx_hooks[2];

		void** stack;

		long current_ret_data;
		long original_ret_data;

		cell* stack_amxx;

		unsigned char* original_code;

		bool call_hooks;

		Allocator allocator;

		Function(void* address, ke::Vector<TypeHandler*>* arguments_handlers, TypeHandler* return_handler);

		virtual ~Function();
		virtual unsigned char* create_trampoline() = 0;

		void clean_hooks();

		void** get_stack_for_arg(int n);
		int get_stack_places();

		void convert_to_amxx(void** stack);

		long call(void** stack);
		long virtual call_original(void** stack) = 0;

		unsigned char* create_trampoline_generic(int stack_fix, bool pass_ecx, bool handle_jump = 1);

		OkapiRet call_amxx_hook(int hook);
		OkapiRet call_amxx_hooks(int phase);

	public:

		AMX_Hook* add_hook(AMX* amx, const char* callback, int phase);
		void del_hook(AMX_Hook* hook);

		static void clean();
		static void clean_all_hooks();
		static AMX_Hook* get_current_hook();

		int get_n_args();
		void set_arg(int n, AMX* amx, cell param);
		void set_return(AMX *amx, cell *params);
		int get_n_ret_params();

		long get_origin_return(AMX *amx, cell *params);

		int call_n_params();
		virtual long call_amx(AMX* amx, cell* params, bool call_hooks);
};

class FunctionMethod : public Function
{
	public:

		FunctionMethod(void* address, ke::Vector<TypeHandler*>* arguments_handlers, TypeHandler* return_handler);
		~FunctionMethod();

	protected:

		unsigned char* create_trampoline();
		long call_original(void** stack);

	private:

		int get_stack_dislocation();
};

class FunctionCdecl : public Function
{
	public:

		FunctionCdecl(void* address, ke::Vector<TypeHandler*>* arguments_handlers, TypeHandler* return_handler);
		~FunctionCdecl();

	protected:

		unsigned char* create_trampoline();
		long call_original(void** stack);

	private:

		int get_stack_dislocation();
};

class FunctionVirtual : public Function
{
	private:

		int get_stack_dislocation();

		void* original_address;

	protected:

		unsigned char* create_trampoline();

		long call_amx(AMX* amx, cell* params, bool call_hooks);
		long call_original(void** stack);

	public:

		FunctionVirtual(void* address, ke::Vector<TypeHandler*>* arguments_handlers, TypeHandler* return_handler);
		~FunctionVirtual();
};

#endif // FUNCTION_H
