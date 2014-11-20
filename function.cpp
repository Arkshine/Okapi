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
#include <CVector.h>
#include <assembly_create.h>
#include <type_handler.h>
#include <memory_.h>

CVector<Function*> Function::functions;
CVector<AMX_Hook*> Function::hooks_stack;

long Function::Gate(void* ebp,void* eip,Function* func, void** stack,...)
{
	return func->call(stack);
}

float Function::GateFloat(void* ebp,void* eip,Function* func, void** stack,...)
{
	return func->call(stack);
}

Function::Function(void* address,CVector<TypeHandler*> arguments_handlers,TypeHandler* return_handler)  : 
																					address(address), 
																					arguments_handlers(arguments_handlers),
																					return_handler(return_handler),
																					trampoline(NULL)
{
	functions.push_back(this);

	stack_amxx = new cell[arguments_handlers.size()];

	this->call_hooks = true;
}

unsigned char* Function::create_trampoline_generic(int stack_fix,bool pass_ecx, bool handle_jump)
{
	if(handle_jump)
	{
		memcpy(original_code,address,8);
	}

	AssemblyCreate assembly;

	Inst_Mov_EAX_VAL* inst_store_fix_part1 = NULL;
	Inst_Mov_EAX_VAL* inst_store_fix_part2 = NULL;

	if(handle_jump)
	{
									assembly.add<Inst_Mov_EDX_VAL>()->set_long((long)address);				// mov edx, address
													
		inst_store_fix_part1	 =	assembly.add<Inst_Mov_EAX_VAL>();										// mov eax, fix_part1
									assembly.add<Inst_Mov_ptrEDX_EAX>();									// mov [edx], eax

		inst_store_fix_part2	 =	assembly.add<Inst_Mov_EAX_VAL>();										// mov eax, fix_part2
									assembly.add<Inst_Mov_ptrEDXpVAL_EAX>()->set_inc(4);					// mov [edx+4], eax
	}
									assembly.add<Inst_Pop_EDX>();											// pop edx
													
	if(pass_ecx)
	{
									assembly.add<Inst_Push_ECX>();											// push ecx
	}

									assembly.add<Inst_Push_ESP>();											// push esp
									assembly.add<Inst_Push_VAL>()->set_long((long)this);					// push this
									assembly.add<Inst_Push_EDX>();											// push edx
									assembly.add<Inst_Enter>();												// enter
	Inst_Call* inst_call		  = assembly.add<Inst_Call>();												// call original
									assembly.add<Inst_Leave>();												// leave
									assembly.add<Inst_Add_ESP_Val>()->set_inc(8 + 4*pass_ecx);				// add esp, 8or12
									assembly.add<Inst_Mov_EDX_ptrESPpVAL>()->set_inc(-(8 + 4*pass_ecx));	// mov edx, [esp-8or12]
									assembly.add<Inst_Mov_ptrESP_EDX>();									// mov [esp], edx
													
									assembly.add<Inst_Mov_EDX_VAL>()->set_long((long)address);				// mov edx, address

	Inst_Mov_ECX_VAL* inst_store_patch_part1 = NULL;
	Inst_Mov_ECX_VAL* inst_store_patch_part2 = NULL;

	if(handle_jump)
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
		
	if(handle_jump)
	{
		jump[0] = 0xE9;
		*((long*)&jump[1]) = (long)block - ((long)address + 5);
	}

	if(handle_jump)
	{
		inst_store_fix_part1->set_long(((long*)address)[0]);
		inst_store_fix_part2->set_long(((long*)address)[1]);
	}

	if(return_handler->uses_st_register())
	{
		inst_call->set_address((long)&Function::GateFloat);
	}
	else
	{
		inst_call->set_address((long)&Function::Gate);
	}
	
	if(handle_jump)
	{
		inst_store_patch_part1->set_long(((long*)jump)[0]);
		inst_store_patch_part2->set_long(((long*)jump)[1]);
	}

	Memory m;

	if(handle_jump)
	{
		m.make_writable_executable((long)address);
		memcpy(address,jump,5);
	}

	m.make_writable_executable((long)block,size);
	
	return block;
}

AMX_Hook* Function::add_hook(AMX *amx, const char* callback, int phase)
{
	AssemblyCreate assembly;

	assembly.add<Inst_Enter>();
								
	int start = (int)this->arguments_handlers.size() - 1;
	int end = 0;

	assembly.add<Inst_Push_VAL>()->set_long( FP_DONE );		
		
	for(int i=start; i>=end; i--)
	{
		assembly.add<Inst_Push_VAL>()->set_long( this->arguments_handlers[i]->get_amx_param_type() );		
	}

	assembly.add<Inst_Push_VAL>()->set_long( (long) callback );
	assembly.add<Inst_Push_VAL>()->set_long( (long) amx );
												
	Inst_Call* inst_call  = assembly.add<Inst_Call>();

	assembly.add<Inst_Add_ESP_Val>()->set_inc(4 * (this->arguments_handlers.size() + 3));	

	assembly.add<Inst_Leave>();

	assembly.add<Inst_Ret>();					
													
	int size = assembly.size();

	unsigned char* block = assembly.get_block();
			
	inst_call->set_address((long)MF_RegisterSPForwardByName);

	Memory m;
	m.make_writable_executable((long)block,size);
		
	long ret = reinterpret_cast<long (*)()>(block)();

	if(ret < 1)
		return 0;

	AMX_Hook* amx_hook = new AMX_Hook(this,ret,phase);

	amx_hooks[phase].push_back(amx_hook);

	return amx_hook;
}

typedef CVector<AMX_Hook*>::iterator it_t;

void Function::del_hook(AMX_Hook* hook)
{
	CVector<AMX_Hook*>& hooks_phase = this->amx_hooks[hook->phase];

	for(it_t it = hooks_phase.begin(); it != hooks_phase.end() ; it++)
	{
		if(*it == hook)
		{
			hooks_phase.erase(it);

			delete hook;

			return;
		}
	}
}

OkapiRet Function::call_amxx_hook(int hook)
{
	AssemblyCreate assembly;

	assembly.add<Inst_Enter>();
								
	int start = (int)this->arguments_handlers.size() - 1;
	int end = 0;

	for(int i=start; i>=end; i--)
	{
		assembly.add<Inst_Push_VAL>()->set_long( this->stack_amxx[i] );		
	}

	assembly.add<Inst_Push_VAL>()->set_long( (long) hook );
												
	Inst_Call* inst_call  = assembly.add<Inst_Call>();

	assembly.add<Inst_Add_ESP_Val>()->set_inc(4 * (this->arguments_handlers.size() + 1));	

	assembly.add<Inst_Leave>();

	assembly.add<Inst_Ret>();					
													
	int size = assembly.size();

	unsigned char* block = assembly.get_block();
			
	inst_call->set_address((long)MF_ExecuteForward);

	Memory m;
	m.make_writable_executable((long)block,size);
		
	return reinterpret_cast<OkapiRet (*)()>(block)();
}

int FunctionMethod::get_stack_dislocation()
{
	int dislocation = 0;

#ifdef __linux__
	size_t i = 0;
#else
	size_t i = 1;
#endif

	for(;i<this->arguments_handlers.size();i++)
	{
		dislocation += arguments_handlers[i]->stack_places() * sizeof(long);
	}

	return dislocation;
}

unsigned char* FunctionMethod::create_trampoline()
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

	return this->create_trampoline_generic(stack_fix,pass_ecx);
}

long FunctionMethod::call_original(void** stack)
{
	float ret_data;

	AssemblyCreate assembly;

	assembly.add<Inst_Enter>();
								
	int start = this->get_stack_dislocation() / 4;

#ifdef __linux__
	int end = 0;
#else
	assembly.add<Inst_Mov_ECX_VAL>()->set_long((long)stack[0]);
	assembly.add<Inst_Push_ECX>();

	int end = 1;
#endif

	for(int i=start; i>=end; i--)
	{
		assembly.add<Inst_Push_VAL>()->set_long((long)stack[i]);		
	}
												
	Inst_Call* inst_call  = assembly.add<Inst_Call>();		// call original

#ifdef __linux__
	assembly.add<Inst_Add_ESP_Val>()->set_inc(this->get_stack_dislocation());	
#endif
		
	if(return_handler->uses_st_register())
	{
		assembly.add<Inst_Fstp>()->set_ptr((long)&ret_data);
	}

	assembly.add<Inst_Leave>();

	assembly.add<Inst_Ret>();
																			
	int size = assembly.size();

	unsigned char* block = assembly.get_block();
			
	inst_call->set_address((long)this->address);

	Memory m;
	m.make_writable_executable((long)block,size);

	long ret = reinterpret_cast<long (*)()>(block)();

	if(!return_handler->uses_st_register())
	{
		return ret;
	}

	return ret_data;
}

FunctionMethod::FunctionMethod(void* address,CVector<TypeHandler*> arguments_handlers,TypeHandler* return_handler) : Function(address,arguments_handlers,return_handler)
{
	original_code = new unsigned char[8];


	this->trampoline = this->create_trampoline();
}

FunctionCdecl::FunctionCdecl(void* address,CVector<TypeHandler*> arguments_handlers,TypeHandler* return_handler) : Function(address,arguments_handlers,return_handler)
{
	original_code = new unsigned char[8];

	this->trampoline = this->create_trampoline();
}

int FunctionCdecl::get_stack_dislocation()
{
	int dislocation = 0;

	for(size_t i = 0;i<this->arguments_handlers.size();i++)
	{
		dislocation += arguments_handlers[i]->stack_places() * sizeof(long);
	}

	return dislocation;
}

unsigned char* FunctionCdecl::create_trampoline()
{
	return this->create_trampoline_generic(0,false);
}

long FunctionCdecl::call_original(void** stack)
{
	float ret_data;

	AssemblyCreate assembly;

	assembly.add<Inst_Enter>();
								
	int start = this->get_stack_dislocation() / 4;

	for(int i=start; i>=0; i--)
	{
		assembly.add<Inst_Push_VAL>()->set_long((long)stack[i]);		
	}
												
	Inst_Call* inst_call  = assembly.add<Inst_Call>();		// call original

	assembly.add<Inst_Add_ESP_Val>()->set_inc(this->get_stack_dislocation());	
		
	if(return_handler->uses_st_register())
	{
		assembly.add<Inst_Fstp>()->set_ptr((long)&ret_data);
	}

	assembly.add<Inst_Leave>();

	assembly.add<Inst_Ret>();
																			
	int size = assembly.size();

	unsigned char* block = assembly.get_block();
			
	inst_call->set_address((long)this->address);

	Memory m;
	m.make_writable_executable((long)block,size);

	long ret = reinterpret_cast<long (*)()>(block)();

	if(!return_handler->uses_st_register())
	{
		return ret;
	}

	return ret_data;
}


long FunctionVirtual::call_original(void** stack)
{
	float ret_data;

	AssemblyCreate assembly;

	assembly.add<Inst_Enter>();
								
	int start = this->get_stack_dislocation() / 4;

#ifdef __linux__
	int end = 0;
#else
	assembly.add<Inst_Mov_ECX_VAL>()->set_long((long)stack[0]);
	assembly.add<Inst_Push_ECX>();

	int end = 1;
#endif

	for(int i=start; i>=end; i--)
	{
		assembly.add<Inst_Push_VAL>()->set_long((long)stack[i]);		
	}
												
	Inst_Call* inst_call  = assembly.add<Inst_Call>();		// call original

#ifdef __linux__
	assembly.add<Inst_Add_ESP_Val>()->set_inc(this->get_stack_dislocation());	
#endif
		
	if(return_handler->uses_st_register())
	{
		assembly.add<Inst_Fstp>()->set_ptr((long)&ret_data);
	}

	assembly.add<Inst_Leave>();

	assembly.add<Inst_Ret>();
																			
	int size = assembly.size();

	unsigned char* block = assembly.get_block();
			
	inst_call->set_address((long)this->original_address);

	Memory m;
	m.make_writable_executable((long)block,size);

	long ret = reinterpret_cast<long (*)()>(block)();

	if(!return_handler->uses_st_register())
	{
		return ret;
	}

	return ret_data;
}

long FunctionVirtual::call_amx(AMX* amx, cell* params,bool call_hooks)
{
	Allocator allocator_c;

	int stack_places = this->get_stack_places();

	void** stack_call = allocator_c.alloc<void*>(stack_places);

	CVector<TypeHandler*>& handlers = this->arguments_handlers;

	int offset = 0;

	for(size_t i=0;i<handlers.size();i++)
	{
		TypeHandler*& handler = handlers[i];

		handler->convert_from_amxx(allocator_c,&stack_call[offset],amx,params[i]);

		offset += handler->stack_places();
	}

	long ret;

	if(call_hooks)
	{
		ret = this->call(stack_call);
	}
	else
	{	
		ret = this->call_original(stack_call);
	}

	return return_handler->convert_to_amxx(amx,&params[handlers.size()],ret);
}
