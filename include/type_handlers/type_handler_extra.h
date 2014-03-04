

#ifndef __TYPE_HANDLER_EXTRA__
#define __TYPE_HANDLER_EXTRA__

#include <type_handler.h>
#include "amxxmodule.h"
#include <globals.h>

class IntHandler : public TypeHandler
{
	char* get_name()
	{
		return "int";
	}

	ForwardParam get_amx_param_type()
	{
		return FP_CELL;
	}

	void convert_to_amxx(Allocator& allocator,void** stack,cell& value)
	{
		value = (cell)(*stack);
	}

	void convert_from_amxx(Allocator& allocator,void** stack,AMX* amx,cell param)
	{
		*stack = (void*)*MF_GetAmxAddr(amx,param);
	}

	long convert_to_amxx(AMX *amx, cell* params,long value)
	{
		return value;
	}

	long convert_from_amxx(Allocator& allocator,AMX *amx, cell* params)
	{
		return (long) *MF_GetAmxAddr(amx,*params);
	}

};

class VoidHandler : public TypeHandler
{
	char* get_name()
	{
		return "void";
	}

	ForwardParam get_amx_param_type()
	{
		return FP_CELL;
	}

	void convert_to_amxx(Allocator& allocator,void** stack,cell& value){}

	void convert_from_amxx(Allocator& allocator,void** stack,AMX* amx,cell param)
	{

	}

	long convert_from_amxx(Allocator& allocator,AMX *amx, cell* params)
	{
		return 0;
	}
	
	long convert_to_amxx(AMX *amx, cell* params,long value)
	{
		return 0;
	}
};

class CBaseHandler : public TypeHandler
{
	char* get_name()
	{
		return "cbase";
	}

	ForwardParam get_amx_param_type()
	{
		return FP_CELL;
	}

	void convert_to_amxx(Allocator& allocator,void** stack,cell& value)
	{
		value = (cell) G_HL_TypeConversion.cbase_to_id((*stack));
	}

	void convert_from_amxx(Allocator& allocator,void** stack,AMX* amx,cell param)
	{
		cell value = *MF_GetAmxAddr(amx,param);
		
		void* converted = G_HL_TypeConversion.id_to_cbase(value);

		*stack = (void*)converted;
	}

	long convert_from_amxx(Allocator& allocator,AMX *amx, cell* params)
	{
		cell value = *MF_GetAmxAddr(amx,*params);

		return (long) G_HL_TypeConversion.id_to_cbase(value);
	}

	long convert_to_amxx(AMX *amx, cell* params,long value)
	{
		return G_HL_TypeConversion.cbase_to_id((void*)value);
	}
};

class FloatHandler : public TypeHandler
{
	char* get_name()
	{
		return "float";
	}

	ForwardParam get_amx_param_type()
	{
		return FP_CELL;
	}

	void convert_to_amxx(Allocator& allocator,void** stack,cell& value)
	{
		value = (cell)(*stack);
	}

	void convert_from_amxx(Allocator& allocator,void** stack,AMX* amx,cell param)
	{
		float* stack_f = (float*) stack;

		*stack_f = amx_ctof(*MF_GetAmxAddr(amx,param));
	}

	virtual bool uses_st_register() 
	{
		return true;
	}

	long convert_from_amxx(Allocator& allocator,AMX *amx, cell* params)
	{
		return amx_ctof(*MF_GetAmxAddr(amx,*params));
	}

	long convert_to_amxx(AMX *amx, cell* params,long value)
	{
		return amx_ftoc(value);
	}
};

class VecHandler : public TypeHandler
{
	char* get_name()
	{
		return "vec";
	}

	ForwardParam get_amx_param_type()
	{
		return FP_ARRAY;
	}

	void convert_to_amxx(Allocator& allocator,void** stack,cell& value)
	{
#ifdef __linux__
		long* stack_l = (long*) stack[0];
#else
		long* stack_l = (long*) stack;
#endif

		cell *amxVector = allocator.alloc<cell>(3);
		
		amxVector[0] = ((cell)(stack_l[0]));
		amxVector[1] = ((cell)(stack_l[1]));
		amxVector[2] = ((cell)(stack_l[2]));

		value = MF_PrepareCellArray(amxVector,3);
	}

	int stack_places()
	{
#ifdef __linux__
		return 1;
#else
		return 3;
#endif
	}

	void convert_from_amxx(Allocator& allocator,void** stack,AMX* amx,cell param)
	{
#ifdef __linux__
		*stack = (float*) allocator.alloc<Vector>(1);

		float* stack_f = (float*) *stack;
#else
		float* stack_f = (float*) stack;
#endif
		cell* amxx_vector = MF_GetAmxAddr(amx,param);	

		stack_f[0] = amx_ctof(amxx_vector[0]);
		stack_f[1] = amx_ctof(amxx_vector[1]);
		stack_f[2] = amx_ctof(amxx_vector[2]);
		
	}

	long convert_from_amxx(Allocator& allocator,AMX *amx, cell* params)
	{
		cell* amxx_vector = MF_GetAmxAddr(amx,*params);

		Vector* vec = allocator.alloc<Vector>(1);

		vec->x = amx_ctof(amxx_vector[0]);
		vec->y = amx_ctof(amxx_vector[1]);
		vec->z = amx_ctof(amxx_vector[2]);

		return (long)vec;
	}

	int by_ref_n_params()
	{
		return 1;
	}

	long convert_to_amxx(AMX *amx, cell* params,long value)
	{
		cell* amxx_vector = MF_GetAmxAddr(amx,*params);

		Vector* vec = (Vector*) value;

		amxx_vector[0] = amx_ftoc(vec->x);
		amxx_vector[1] = amx_ftoc(vec->y);
		amxx_vector[2] = amx_ftoc(vec->z);

		return 0;
	}
};

class EntvarsHandler : public TypeHandler
{
	char* get_name()
	{
		return "entvars";
	}

	ForwardParam get_amx_param_type()
	{
		return FP_CELL;
	}

	void convert_to_amxx(Allocator& allocator,void** stack,cell& value)
	{
		value = (cell)(G_HL_TypeConversion.entvar_to_id((entvars_t*)*stack));
	}

	void convert_from_amxx(Allocator& allocator,void** stack,AMX* amx,cell param)
	{
		cell value = *MF_GetAmxAddr(amx,param);
		
		void* converted = G_HL_TypeConversion.id_to_entvar(value);

		*stack = (void*)converted;
	}

	long convert_from_amxx(Allocator& allocator,AMX *amx, cell* params)
	{
		cell value = *MF_GetAmxAddr(amx,*params);
		
		void* converted = G_HL_TypeConversion.id_to_entvar(value);

		return (long)converted;
	}

	long convert_to_amxx(AMX *amx, cell* params,long value)
	{
		return (long) G_HL_TypeConversion.entvar_to_id((entvars_t*)value);
	}
};

class StrHandler : public TypeHandler
{
	char* get_name()
	{
		return "string";
	}

	ForwardParam get_amx_param_type()
	{
		return FP_ARRAY;
	}

	void convert_to_amxx(Allocator& allocator,void** stack,cell& value)
	{
		char* str = (char*)*stack;

		if(str)
		{
			value = MF_PrepareCharArray((char*)str, strlen((char *)str)+1);			
		}
		else
		{
			value = MF_PrepareCharArray("",1);
		}
	}

	void convert_from_amxx(Allocator& allocator,void** stack,AMX* amx,cell param)
	{
		int len;
		char *str = g_fn_GetAmxString(amx,param,0,&len);
		char *allocString = allocator.alloc<char>(len+1);
		strcpy(allocString,str);

		*stack = (void*)allocString;
	}

	long convert_from_amxx(Allocator& allocator,AMX *amx, cell* params)
	{
		int len;
		char *str = g_fn_GetAmxString(amx,*params,0,&len);
		char *allocString = allocator.alloc<char>(len+1);
		strcpy(allocString,str);

		return (long)allocString;
	}

	virtual int by_ref_n_params()
	{
		return 2;
	}

	long convert_to_amxx(AMX *amx, cell* params,long value)
	{
		g_fn_SetAmxString(amx,params[0],(const char *)value,*MF_GetAmxAddr(amx,params[1]));

		return 0;
	}
};


class EdictHandler : public TypeHandler
{
	char* get_name()
	{
		return "edict";
	}

	ForwardParam get_amx_param_type()
	{
		return FP_CELL;
	}

	void convert_to_amxx(Allocator& allocator,void** stack,cell& value)
	{
		value = (cell)	ENTINDEX((edict_t*)*stack);
	}

	void convert_from_amxx(Allocator& allocator,void** stack,AMX* amx,cell param)
	{
		cell value = *MF_GetAmxAddr(amx,param);
		
		void* converted = INDEXENT(value);

		*stack = (void*)converted;
	}

	long convert_from_amxx(Allocator& allocator,AMX *amx, cell* params)
	{
		cell value = *MF_GetAmxAddr(amx,*params);
		
		void* converted = INDEXENT(value);

		return (long)converted;
	}

	long convert_to_amxx(AMX *amx, cell* params,long value)
	{
		return (long) ENTINDEX((edict_t*)value);
	}
};

class VecPtrHandler : public TypeHandler
{
	char* get_name()
	{
		return "vec_ptr";
	}

	ForwardParam get_amx_param_type()
	{
		return FP_ARRAY;
	}

	void convert_to_amxx(Allocator& allocator,void** stack,cell& value)
	{
		long* stack_l = (long*) stack[0];

		cell *amxVector = allocator.alloc<cell>(3);
		
		amxVector[0] = ((cell)(stack_l[0]));
		amxVector[1] = ((cell)(stack_l[1]));
		amxVector[2] = ((cell)(stack_l[2]));

		value = MF_PrepareCellArray(amxVector,3);
	}

	void convert_from_amxx(Allocator& allocator,void** stack,AMX* amx,cell param)
	{
		Vector*& vec = (Vector*&)*stack;

		vec = allocator.alloc<Vector>(1);

		cell* amxx_vector = MF_GetAmxAddr(amx,param);	

		vec->x = amx_ctof(amxx_vector[0]);
		vec->y = amx_ctof(amxx_vector[1]);
		vec->z = amx_ctof(amxx_vector[2]);
		
	}

	long convert_from_amxx(Allocator& allocator,AMX *amx, cell* params)
	{
		cell* amxx_vector = MF_GetAmxAddr(amx,*params);

		Vector* vec = allocator.alloc<Vector>(1);

		vec->x = amx_ctof(amxx_vector[0]);
		vec->y = amx_ctof(amxx_vector[1]);
		vec->z = amx_ctof(amxx_vector[2]);

		return (long)vec;
	}

	int by_ref_n_params()
	{
		return 1;
	}

	long convert_to_amxx(AMX *amx, cell* params,long value)
	{
		cell* amxx_vector = MF_GetAmxAddr(amx,*params);

		Vector* vec = (Vector*) value;

		amxx_vector[0] = amx_ftoc(vec->x);
		amxx_vector[1] = amx_ftoc(vec->y);
		amxx_vector[2] = amx_ftoc(vec->z);

		return 0;
	}
};

#endif

