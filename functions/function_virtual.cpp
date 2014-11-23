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

#include "function.h"

FunctionVirtual::FunctionVirtual(void* address, ke::Vector<TypeHandler*>* arguments_handlers, TypeHandler* return_handler) : Function(address, arguments_handlers, return_handler)
{
	original_address = *((void**)address);

	original_code = NULL;

	this->trampoline = this->create_trampoline();

	{
		Memory m;

		int prot = m.get_memory_protection((long)address);

		m.make_writable((long)address, 4);

		*((void**)address) = this->trampoline;

		m.set_memory_protection((long)address, prot);
	}
}

FunctionVirtual::~FunctionVirtual()
{
	Memory m;

	int prot = m.get_memory_protection((long)address);

	m.make_writable((long)address, 4);

	*((void**)address) = original_address;

	m.set_memory_protection((long)address, prot);
}

long FunctionVirtual::call_original(void** stack)
{
	float ret_data;

	AssemblyCreate assembly;

	assembly.add<Inst_Enter>();

	size_t start = this->get_stack_dislocation() / 4;

#ifdef __linux__
	size_t end = 0;
#else
	assembly.add<Inst_Mov_ECX_VAL>()->set_long((long)stack[0]);
	assembly.add<Inst_Push_ECX>();

	size_t end = 1;
#endif

	for (size_t i = start; i >= end; --i)
	{
		assembly.add<Inst_Push_VAL>()->set_long((long)stack[i]);
	}

	Inst_Call* inst_call  = assembly.add<Inst_Call>();		// call original

#ifdef __linux__
	assembly.add<Inst_Add_ESP_Val>()->set_inc(this->get_stack_dislocation());
#endif

	if (return_handler->uses_st_register())
	{
		assembly.add<Inst_Fstp>()->set_ptr((long)&ret_data);
	}

	assembly.add<Inst_Leave>();

	assembly.add<Inst_Ret>();

	int size = assembly.size();

	unsigned char* block = assembly.get_block();

	inst_call->set_address((long)this->original_address);

	Memory m;
	m.make_writable_executable((long)block, size);

	long ret = reinterpret_cast<long(*)()>(block)();

	if (!return_handler->uses_st_register())
	{
		return ret;
	}

	return ret_data;
}

long FunctionVirtual::call_amx(AMX* amx, cell* params, bool call_hooks)
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

	long ret;

	if (call_hooks)
	{
		ret = this->call(stack_call);
	}
	else
	{
		ret = this->call_original(stack_call);
	}

	return return_handler->convert_to_amxx(amx, &params[handlers->length()], ret);
}

int FunctionVirtual::get_stack_dislocation()
{
	int dislocation = 0;

#ifdef __linux__
	size_t i = 0;
#else
	size_t i = 1;
#endif

	for (; i < this->arguments_handlers->length(); ++i)
	{
		dislocation += arguments_handlers->at(i)->stack_places() * sizeof(long);
	}

	return dislocation;
}

unsigned char* FunctionVirtual::create_trampoline()
{
#ifdef __linux__
	int stack_fix = 0;
#else
	int stack_fix = this->get_stack_dislocation();
#endif

#ifdef __linux__
	bool pass_ecx = 0;
#else
	bool pass_ecx = 1;
#endif

	return this->create_trampoline_generic(stack_fix, pass_ecx, false);
}