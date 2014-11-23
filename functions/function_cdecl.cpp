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

FunctionCdecl::FunctionCdecl(void* address, ke::Vector<TypeHandler*>* arguments_handlers, TypeHandler* return_handler) : Function(address, arguments_handlers, return_handler)
{
	original_code = new unsigned char[8];

	this->trampoline = this->create_trampoline();
}

FunctionCdecl::~FunctionCdecl()
{
	memcpy(address, original_code, 8);
}

int FunctionCdecl::get_stack_dislocation()
{
	int dislocation = 0;

	for (size_t i = 0; i < this->arguments_handlers->length(); ++i)
	{
		dislocation += arguments_handlers->at(i)->stack_places() * sizeof(long);
	}

	return dislocation;
}

unsigned char* FunctionCdecl::create_trampoline()
{
	return this->create_trampoline_generic(0, false);
}

long FunctionCdecl::call_original(void** stack)
{
	float ret_data;

	AssemblyCreate assembly;

	assembly.add<Inst_Enter>();

	int start = this->get_stack_dislocation() / 4;

	for (int i = start; i >= 0; --i)
	{
		assembly.add<Inst_Push_VAL>()->set_long((long)stack[i]);
	}

	Inst_Call* inst_call  = assembly.add<Inst_Call>();		// call original

	assembly.add<Inst_Add_ESP_Val>()->set_inc(this->get_stack_dislocation());

	if (return_handler->uses_st_register())
	{
		assembly.add<Inst_Fstp>()->set_ptr((long)&ret_data);
	}

	assembly.add<Inst_Leave>();
	assembly.add<Inst_Ret>();

	int size = assembly.size();

	unsigned char* block = assembly.get_block();

	inst_call->set_address((long)this->address);

	Memory m;
	m.make_writable_executable((long)block, size);

	long ret = reinterpret_cast<long(*)()>(block)();

	if (!return_handler->uses_st_register())
	{
		return ret;
	}

	return ret_data;
}