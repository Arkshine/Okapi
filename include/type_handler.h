
#ifndef __TYPE_HANDLER__
#define __TYPE_HANDLER__

#include <allocator.h>

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

		virtual long convert_from_amxx(Allocator& allocator,AMX *amx, cell* params) = 0;

		virtual long convert_to_amxx(AMX *amx, cell* params,long value) = 0;

		virtual ForwardParam get_amx_param_type() = 0;
		
		virtual char* get_name() = 0;
		
		virtual void convert_to_amxx(Allocator& allocator,void** stack,cell& value) = 0;

		virtual void convert_from_amxx(Allocator& allocator,void** stack,AMX* amx,cell param) = 0;
};

#endif
