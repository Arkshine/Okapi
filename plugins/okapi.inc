
#if defined _okapi_included
	#endinput
#endif
#define _okapi_included

#if AMXX_VERSION_NUM >= 175
 #pragma reqlib okapi
 #if !defined AMXMODX_NOAUTOLOAD
  #pragma loadlib okapi
 #endif
#else
 #pragma library okapi
#endif

#include <okapi_const>

/**
 * Gets the base address of where the provided library is allocated in memory
 *  
 * @return					the base address
 *
 */
native okapi_get_base_ptr(const lib_name[])

	/**
	 * Gets the base address of where the mod library is allocated in memory
	 *  
	 * @return					the base address
	 *
	 */
	stock okapi_mod_get_base_ptr()
	{
		static address; address || (address = okapi_get_base_ptr("mod"));
		return address;
	}

	/**
	 * Gets the base address of where the engine library is allocated in memory
	 *  
	 * @return					the base address
	 *
	 */
	stock okapi_engine_get_base_ptr()
	{
		static address; address || (address = okapi_get_base_ptr("engine"));
		return address;
	}


/**
 *  Searches for a signature in the mod library, starting at 'ptr' address
 *	For bytes to ignore, use "𐌻" or any number above 0xFF like:
 *
 *	{0x51,0x56,"𐌻","𐌻",0x8B,0x86}
 *	{0x51,0x56,0xDEF,0xDEF,0x8B,0x86}
 *
 *	I chose the character 𐌻 because it looks like a lambda but it is made of 4 chars
 *	so, it only "forbids" using the sequence {0xF0,0x90,0x8C,0xBB,0x00} in signatures
 *  
 * @param base_address      the library address
 * @param start_address		the start address
 * @param signature[]		the signature to find
 * @param size				the size of the signature
 * @return					the address where the sig was first found
 *							0 if none was found
 */
native okapi_find_sig_at(lib_address, start_address, const signature[], count = sizeof signature);

	/**
	 * Searches for a signature in the mod library from 'start_address'.
	 *
	 * @param lib_address		the library address
	 * @param signature[]		the signature to find
	 * @param count				the size of the signature
	 * @return					the address where the sig was first found
	 *							0 if none was found
	 */
	stock okapi_find_sig(lib_address, const signature[], count = sizeof signature)
	{
		return okapi_find_sig_at(lib_address, lib_address, signature, count);
	}

	/**
	 * Searches for a signature in the mod library from 'start_address'.
	 *
	 * @param start_address		the start address
	 * @param signature[]		the signature to find
	 * @param count				the size of the signature
	 * @return					the address where the sig was first found
	 *							0 if none was found
	 */
	stock okapi_mod_find_sig_at(start_address, const signature[], count = sizeof signature)
	{
		return okapi_find_sig_at(okapi_mod_get_base_ptr(), start_address, signature, count);
	}

	/**
	 * Searches for a signature in the mod library.
	 *
	 * @param start_address		the start address
	 * @param signature[]		the signature to find
	 * @param count				the size of the signature
	 * @return					the address where the sig was first found
	 *							0 if none was found
	 */
	stock okapi_mod_find_sig(const signature[], count = sizeof signature)
	{
		return okapi_mod_find_sig_at(okapi_mod_get_base_ptr(), signature, count);
	}

	/**
	 * Searches for a signature in the engine library from 'start_address'.
	 *
	 * @param start_address		the start address
	 * @param signature[]		the signature to find
	 * @param count				the size of the signature
	 * @return					the address where the sig was first found
	 *							0 if none was found
	 */
	stock okapi_engine_find_sig_at(start_address, const signature[], count = sizeof signature)
	{
		return okapi_find_sig_at(okapi_engine_get_base_ptr(), start_address, signature, count);
	}

	/**
	 * Searches for a signature in the engine library.
	 *
	 * @param start_address		the start address
	 * @param signature[]		the signature to find
	 * @param count				the size of the signature
	 * @return					the address where the sig was first found
	 *							0 if none was found
	 */
	stock okapi_engine_find_sig(const signature[], count = sizeof signature)
	{
		return okapi_engine_find_sig_at(okapi_engine_get_base_ptr(), signature, count);
	}


/**
 *  Attaches okapi to a method (class member function) so you can hook it and call it
 *  
 * @param ptr				the address of the method
 * @param okapi_arg:ret		the return type of the method
 * @param okapi_arg:arg		the type of the method class
 * @param okapi_arg:...		the rest of the types for the arguments of the function
 *
 * @return					an handler to the function attached
 *
 */
native okapi_func:okapi_build_method(ptr,okapi_arg:ret,okapi_arg:class,okapi_arg:...)

/**
 *  Attaches okapi to a function so you can hook it and call it
 *  
 * @param ptr				the address of the method
 * @param okapi_arg:ret		the return type of the method
 * @param okapi_arg:...		the rest of the types for the arguments of the function
 *
 * @return					an handler to the function attached
 *
 */
native okapi_func:okapi_build_function(ptr,okapi_arg:ret,okapi_arg:...)


/**
 *  Attaches okapi to a virtual function of an entity so you can hook it and call it
 *
 * Note: you don't need to pass arg_cbase in the argument list for the entity itself 
 *
 * @id						the id of the entity
 * @offset					the offset of the function in the virtual table
 * @param okapi_arg:ret		the return type of the method
 * @param okapi_arg:...		the rest of the types for the arguments of the function
 *
 * @return					an handler to the function attached
 *
 */
native okapi_func:okapi_build_vfunc_cbase(id,offset,okapi_arg:ret,okapi_arg:...)

/**
 *  Attaches okapi to a virtual function of an entity (using its class) so you can hook it and call it
 *
 * Note: you don't need to pass arg_cbase in the argument list for the entity itself 
 *
 * @classname[]				the name of the class
 * @offset					the offset of the function in the virtual table
 * @param okapi_arg:ret		the return type of the method
 * @param okapi_arg:...		the rest of the types for the arguments of the function
 *
 * @return					an handler to the function attached
 *
 */
native okapi_func:okapi_build_vfunc_class(classname[],offset,okapi_arg:ret,okapi_arg:...)

/**
 *  Attaches okapi to a virtual function of an object so you can hook it and call it
 *
 *
 * @ptr						the address of the object
 * @offset					the offset of the function in the virtual table
 * @param okapi_arg:ret		the return type of the method
 * @param okapi_arg:...		the rest of the types for the arguments of the function
 *
 * @return					an handler to the function attached
 *
 */
native okapi_func:okapi_build_vfunc_ptr(ptr,offset,okapi_arg:ret,okapi_arg:...)


/**
 *  Adds a hook to a previously attached function
 *  
 * @param okapi_func:func	a handler to the function returned by an attach function
 * @param callback[]		the name of the public function in the code that will be 
 *
 * @return					an handler to the hook
 *
 */
native okapi_hook:okapi_add_hook(okapi_func:func,callback[],post=0)

/**
 *  Removes a hook from the function
 *  
 * @param hook				the handler to the hook
 *
 */
native okapi_del_hook(&okapi_hook:hook)

/**
 *  Removes current hook from the function
 *  This is meant to be used inside a hook (when you want to remove it there)
 */
native okapi_del_current_hook()

/**
 *  Modifies a parameter that will be passed in the call to the original function
 *  
 * @param param_id			the index of the parameter. First is 1
 * @param any:...			the value of the parameter
 *
 */
native okapi_set_param(param_id,any:...)

/**
 *  Modifies the return of the function
 *  
 * @param any:...			the value of the return
 *
 */
native okapi_set_return(any:...)

/**
 *  Retrieves the value that the hooked function retrieved
 *  
 * @param any:...			the value of the original return, if passed by ref
 * @return					the value of the original return, if passed by value
 *
 */
native okapi_get_orig_return(any:...)

/**
 *  Calls a function without calling its hooks
 *  
 * @param any:...			the arguments to the function and return variables, if the function returns byref
 * @return					the value that the function returned, if the function doesn't return by ref
 *
 */
native okapi_call(okapi_func:func,any:...)

/**
 *  Calls a function and its hooks
 *  
 * @param any:...			the arguments to the function and return variables, if the function returns byref
 * @return					the value that the function returned, if the function doesn't return by ref
 *
 */
native okapi_call_ex(okapi_func:func,any:...)

/**
 *    Gets the hook that is currently being called.
 *	This is meant to be used inside a hook (when you want to remove it there)
 *  
 * @return					the handler to the hook
 *
 */
native okapi_hook:okapi_current_hook()

/**
 * Gets the offset of an address relative to the engine library
 * 
 * @param ptr			an address
 * @return				the offset of the address in the engine library
 *						0 if the address is not located in the engine library
 */
native okapi_engine_get_ptr_offset(ptr)

/**
 * Gets the offset of an address relative to the mod library
 * 
 * @param ptr			an address
 * @return				the offset of the address in the mod library
 *						0 if the address is not located in the mod library
 */
native okapi_mod_get_ptr_offset(ptr)


/**
 * Gets the address of an offset relative to the engine library
 * 
 * @param offset		an offset
 * @return				the address of the offset in the engine library
 *						0 if the offset is not located in the engine library
 */
native okapi_engine_get_offset_ptr(offset)

/**
 * Gets the address of an offset relative to the mod library
 * 
 * @param offset		an offset
 * @return				the address of the address in the mod library
 *						0 if the offset is not located in the mod library
 */
native okapi_mod_get_offset_ptr(offset)

/**
 * Gets the address of a function, given it's symbolic name
 * 
 * @param symbol		the symbolic name of a function
 * @return				the address of the function
 *						0 if it doesn't exist
 */
native okapi_engine_get_symbol_ptr(const symbol[])

/**
 * Gets the address of a function, given it's symbolic name
 * 
 * @param symbol		the symbolic name of a function
 * @return				the address of the function
 *						0 if it doesn't exist
 */
native okapi_mod_get_symbol_ptr(const symbol[])


/**
 *   Return a if server is windows, returns b if server is linux
 *  
 * @param a					value for windows
 * @param b					value for linux
 *
 * @return					a if windows, b if linux
 *
 */
native wl(a,b)