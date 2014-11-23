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

#include <function.h>
#include "amxxmodule.h"
#include "assembly_create.h"
#include "type_handler.h"
#include "memory_.h"

ke::Vector<Function*> Function::functions;
ke::Vector<AMX_Hook*> Function::hooks_stack;

long Function::Gate(void* ebp, void* eip, Function* func, void** stack, ...)
{
	return func->call(stack);
}

float Function::GateFloat(void* ebp, void* eip, Function* func, void** stack, ...)
{
	return func->call(stack);
}

Function::Function(void* address, ke::Vector<TypeHandler*>* arguments_handlers, TypeHandler* return_handler) :
					address(address), arguments_handlers(arguments_handlers), return_handler(return_handler), trampoline(NULL)
{
	functions.append(this);

	stack_amxx = new cell[arguments_handlers->length()];

	this->call_hooks = true;
}

unsigned char* Function::create_trampoline_generic(int stack_fix, bool pass_ecx, bool handle_jump)
{
	if (handle_jump)
	{
		memcpy(original_code, address, 8);
	}

	AssemblyCreate assembly;

	Inst_Mov_EAX_VAL* inst_store_fix_part1 = NULL;
	Inst_Mov_EAX_VAL* inst_store_fix_part2 = NULL;

	if (handle_jump)
	{
									assembly.add<Inst_Mov_EDX_VAL>()->set_long((long)address);				// mov edx, address

		inst_store_fix_part1	 =	assembly.add<Inst_Mov_EAX_VAL>();										// mov eax, fix_part1
									assembly.add<Inst_Mov_ptrEDX_EAX>();									// mov [edx], eax

		inst_store_fix_part2	 =	assembly.add<Inst_Mov_EAX_VAL>();										// mov eax, fix_part2
									assembly.add<Inst_Mov_ptrEDXpVAL_EAX>()->set_inc(4);					// mov [edx+4], eax
	}
									assembly.add<Inst_Pop_EDX>();											// pop edx

	if (pass_ecx)
	{
									assembly.add<Inst_Push_ECX>();											// push ecx
	}

									assembly.add<Inst_Push_ESP>();											// push esp
									assembly.add<Inst_Push_VAL>()->set_long((long)this);					// push this
									assembly.add<Inst_Push_EDX>();											// push edx
									assembly.add<Inst_Enter>();												// enter
	Inst_Call* inst_call		  = assembly.add<Inst_Call>();												// call original
									assembly.add<Inst_Leave>();												// leave
									assembly.add<Inst_Add_ESP_Val>()->set_inc(8 + 4 * pass_ecx);			// add esp, 8or12
									assembly.add<Inst_Mov_EDX_ptrESPpVAL>()->set_inc(-(8 + 4 * pass_ecx));	// mov edx, [esp-8or12]
									assembly.add<Inst_Mov_ptrESP_EDX>();									// mov [esp], edx

									assembly.add<Inst_Mov_EDX_VAL>()->set_long((long)address);				// mov edx, address

	Inst_Mov_ECX_VAL* inst_store_patch_part1 = NULL;
	Inst_Mov_ECX_VAL* inst_store_patch_part2 = NULL;

	if (handle_jump)
	{
		inst_store_patch_part1	=	assembly.add<Inst_Mov_ECX_VAL>();										// mov eax, patch_part1
									assembly.add<Inst_Mov_ptrEDX_ECX>();									// mov [edx], eax

		inst_store_patch_part2	=	assembly.add<Inst_Mov_ECX_VAL>();										// mov eax, patch_part2
									assembly.add<Inst_Mov_ptrEDXpVAL_ECX>()->set_inc(4);					// mov [edx+4], eax
	}
									assembly.add<Inst_RetN>()->set_count(stack_fix);						// retn stack_fix

	int size = assembly.size();

	unsigned char* block = assembly.create_block();
	unsigned char jump[8];

	if (handle_jump)
	{
		jump[0] = 0xE9;
		*((long*)&jump[1]) = (long)block - ((long)address + 5);

		inst_store_fix_part1->set_long(((long*)address)[0]);
		inst_store_fix_part2->set_long(((long*)address)[1]);
	}

	if (return_handler->uses_st_register())
	{
		inst_call->set_address((long)&Function::GateFloat);
	}
	else
	{
		inst_call->set_address((long)&Function::Gate);
	}

	Memory m;

	if (handle_jump)
	{
		inst_store_patch_part1->set_long(((long*)jump)[0]);
		inst_store_patch_part2->set_long(((long*)jump)[1]);

		m.make_writable_executable((long)address);
		memcpy(address, jump, 5);
	}

	m.make_writable_executable((long)block, size);

	return block;
}

AMX_Hook* Function::add_hook(AMX *amx, const char* callback, int phase)
{
	AssemblyCreate assembly;

	assembly.add<Inst_Enter>();

	size_t start = this->arguments_handlers->length() - 1;
	size_t end = 0;

	assembly.add<Inst_Push_VAL>()->set_long(FP_DONE);

	for (size_t i = start; i != end; --i)
	{
		assembly.add<Inst_Push_VAL>()->set_long(this->arguments_handlers->at(i)->get_amx_param_type());
	}

	assembly.add<Inst_Push_VAL>()->set_long((long)callback);
	assembly.add<Inst_Push_VAL>()->set_long((long)amx);

	Inst_Call* inst_call  = assembly.add<Inst_Call>();

	assembly.add<Inst_Add_ESP_Val>()->set_inc(4 * (this->arguments_handlers->length() + 3));
	assembly.add<Inst_Leave>();
	assembly.add<Inst_Ret>();

	int size = assembly.size();

	unsigned char* block = assembly.get_block();

	inst_call->set_address((long)MF_RegisterSPForwardByName);

	Memory m;
	m.make_writable_executable((long)block, size);

	long ret = reinterpret_cast<long(*)()>(block)();

	if (ret < 1)
		return 0;

	AMX_Hook* amx_hook = new AMX_Hook(this, ret, phase);

	amx_hooks[phase].append(amx_hook);

	return amx_hook;
}

void Function::del_hook(AMX_Hook* hook)
{
	ke::Vector<AMX_Hook*>& hooks_phase = this->amx_hooks[hook->phase];

	for (size_t iter = 0; iter < hooks_phase.length(); ++iter)
	{
		if (hooks_phase[iter] == hook)
		{
			hooks_phase.remove(iter);

			delete hook;

			return;
		}
	}
}

void Function::clean_hooks()
{
	for (size_t i = 0; i < 2; ++i)
	{
		ke::Vector<AMX_Hook*>& amx_hooks_phase = amx_hooks[i];

		for (size_t j = 0; j < amx_hooks_phase.length(); ++j)
		{
			AMX_Hook* amx_hook = amx_hooks_phase[j];
			delete amx_hook;
		}

		amx_hooks_phase.clear();
	}
}

void** Function::get_stack_for_arg(int n)
{
	ke::Vector<TypeHandler*>* handlers = this->arguments_handlers;

	int offset = 0;

	for (size_t i = 0; i < (size_t)n; ++i)
	{
		TypeHandler*& handler = handlers->at(i);

		offset += handler->stack_places();
	}

	return &stack[offset];
}

int Function::get_stack_places()
{
	int n = 0;

	ke::Vector<TypeHandler*>* handlers = this->arguments_handlers;

	for (size_t i = 0; i < handlers->length(); ++i)
	{
		TypeHandler*& handler = handlers->at(i);

		n += handler->stack_places();
	}

	return n;
}

void Function::convert_to_amxx(void** stack)
{
	ke::Vector<TypeHandler*>* handlers = this->arguments_handlers;

	int offset = 0;

	for (size_t i = 0; i < handlers->length(); ++i)
	{
		TypeHandler*& handler = handlers->at(i);

		handler->convert_to_amxx(allocator, &stack[offset], this->stack_amxx[i]);

		offset += handler->stack_places();
	}
}

long Function::call(void** stack)
{
	this->stack = stack;
	this->current_ret_data = 0;
	this->original_ret_data = 0;

	if (call_hooks)
	{
		convert_to_amxx(stack);
	}

	OkapiRet amxx_ret = OkapiRetIgnore;

	if (call_hooks)
	{
		amxx_ret = call_amxx_hooks(0);
	}

	if (amxx_ret != OkapiRetSupercede)
	{
		this->original_ret_data = call_original(stack);
	}

	if (amxx_ret == OkapiRetIgnore)
	{
		this->current_ret_data = this->original_ret_data;
	}

	if (call_hooks && (amxx_ret != OkapiRetSupercede))
	{
		call_amxx_hooks(1);
	}

	this->stack = NULL;
	allocator.clear();

	return this->current_ret_data;
}

OkapiRet Function::call_amxx_hook(int hook)
{
	AssemblyCreate assembly;

	assembly.add<Inst_Enter>();

	size_t start = this->arguments_handlers->length() - 1;

	for (size_t i = start; i != 0; i--)
	{
		assembly.add<Inst_Push_VAL>()->set_long(this->stack_amxx[i]);
	}

	assembly.add<Inst_Push_VAL>()->set_long((long)hook);

	Inst_Call* inst_call  = assembly.add<Inst_Call>();

	assembly.add<Inst_Add_ESP_Val>()->set_inc(4 * (this->arguments_handlers->length() + 1));
	assembly.add<Inst_Leave>();
	assembly.add<Inst_Ret>();

	int size = assembly.size();

	unsigned char* block = assembly.get_block();

	inst_call->set_address((long)MF_ExecuteForward);

	Memory m;
	m.make_writable_executable((long)block, size);

	return reinterpret_cast<OkapiRet(*)()>(block)();
}

OkapiRet Function::call_amxx_hooks(int phase)
{
	OkapiRet ret = OkapiRetIgnore;

	ke::Vector<AMX_Hook*>& hooks = this->amx_hooks[phase];

	for (size_t i = 0; i < hooks.length(); ++i)
	{
		AMX_Hook* hook = hooks[i];

		hooks_stack.append(hook);

		OkapiRet ret_hook = call_amxx_hook(hook->amx_hook);

		ret = (OkapiRet)ke::Max(ret, ret_hook);

		hooks_stack.pop();

		if (ret == OkapiRetSupercede)
		{
			return ret;
		}
	}

	return ret;
}

void Function::clean()
{
	clean_all_hooks();

	for (size_t i = 0; i < functions.length(); ++i)
	{
		Function* function = functions[i];

		delete function;
	}

	functions.clear();
}

void Function::clean_all_hooks()
{
	for (size_t i = 0; i < functions.length(); ++i)
	{
		Function* function = functions[i];

		function->clean_hooks();
	}
}

AMX_Hook* Function::get_current_hook()
{
	if (!hooks_stack.length())
	{
		return NULL;
	}

	return hooks_stack[hooks_stack.length() - 1];
}

int Function::get_n_args()
{
	return this->arguments_handlers->length();
}

void Function::set_arg(int n, AMX* amx, cell param)
{
	void** stack_arg = get_stack_for_arg(n);

	this->arguments_handlers->at(n)->convert_from_amxx(allocator, stack_arg, amx, param);
}

void Function::set_return(AMX *amx, cell *params)
{
	this->current_ret_data = this->return_handler->convert_from_amxx(allocator, amx, params);
}

int Function::get_n_ret_params()
{
	return this->return_handler->by_ref_n_params();
}

long Function::get_origin_return(AMX *amx, cell *params)
{
	return this->return_handler->convert_to_amxx(amx, params, this->original_ret_data);
}

int Function::call_n_params()
{
	return this->arguments_handlers->length() + this->return_handler->by_ref_n_params();
}

long Function::call_amx(AMX* amx, cell* params, bool call_hooks)
{
	Allocator allocator_c;

	int stack_places = this->get_stack_places();

	void** stack_call = allocator_c.alloc<void*>(stack_places);

	ke::Vector<TypeHandler*>* handlers = this->arguments_handlers;

	int offset = 0;

	for (size_t i = 0; i < handlers->length(); ++i)
	{
		TypeHandler*& handler = handlers->at(i);

		handler->convert_from_amxx(allocator_c, &stack_call[offset], amx, params[i]);

		offset += handler->stack_places();
	}

	this->call_hooks = call_hooks;

	long ret = this->call_original(stack_call);

	this->call_hooks = true;

	return return_handler->convert_to_amxx(amx, &params[handlers->length()], ret);
}
