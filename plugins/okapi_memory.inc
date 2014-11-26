
#if defined _okapi_memory_included
	#endinput
#endif
#define _okapi_memory_included


/**
 * Retrieves the value in an address.
 *
 * @param address	The address where is located the value
 * @param type		The value data type, See okapi_mem_* constants
 * @param ...		If zero additional parameters are provided, the function
 *                  will return an integer or float value directly. 
 *                  If one additional parameter is provided, the function will
 *                  store an integer as float, a float or vector.
 *                  If two additional parameters are provided, the function
 *                  will copy a string or an array to the buffer provided 
 *                  in the second parameter, using the third as the maximum
 *				    buffer size.
 *					
 * @return			If zero additional parameters are provided, the function
 *                  will return an integer or float value.
 *                  If two additional parameters are provided and data typ is
 *                  a string, the function will return the number of cells 
 *                  written to the buffer.
 *
 * @error           Invalid return type
 *                  Incorrect parameters count
 */
native any:okapi_mem_get(address, type, any:...);

/**
 * Sets the value of the location pointed by the address.
 *
 * @param address	The address where is located the value
 * @param type		The value data type, See okapi_mem_* constants
 * @param ...       The value to set. If parameter is an array, 
 *                  its size is required as second parameter.
 *
 * @noreturn
 *
 * @error           Incorrect parameters count
 */
native okapi_mem_set(address, type, any:...);


/**
 *   Replaces every string that occurs in the engine library with another one
 *	the replacement string should be of equal or shorter size than the original
 *	if you know what you are doing and want to skip this check, make force=1
 *  
 * @param str_orig[]		the original string
 * @param str_replace[]		the replacement string
 * @param force				1 will skip the restrition of the string replacement size
 * @return					the number of replacements ocurred
 *
 */
native okapi_engine_replace_string(str_orig[],str_replace[],force=0)

/**
 *   Replaces every string that occurs in the mod library with another one
 *	the replacement string should be of equal or shorter size than the original
 *	if you know what you are doing and want to skip this check, make force=1
 *  
 * @param str_orig[]		the original string
 * @param str_replace[]		the replacement string
 * @param force				1 will skip the restrition of the string replacement size
 * @return					the number of replacements ocurred
 *
 */
native okapi_mod_replace_string(str_orig[],str_replace[],force=0)

/**
 *   Replaces every ocurrence of the sequence of bytes given, in the engine library, with other sequence
 *  
 * @param arr_orig[]		the original string
 * @param arr_replace[]		the replacement string
 * @param count				number of bytes
 * @return					the number of replacements ocurred
 *
 */
native okapi_engine_replace_array(arr_orig[],arr_replace[],len)

/**
 *   Replaces every ocurrence of the sequence of bytes given, in the mod library, with other sequence
 *  
 * @param arr_orig[]		the original string
 * @param arr_replace[]		the replacement string
 * @param count				number of bytes
 * @return					the number of replacements ocurred
 *
 */
native okapi_mod_replace_array(arr_orig[],arr_replace[],len)

/**
 *   Replaces every ocurrence of the integer given, in the engine library, with other integer
 *  
 * @param val_search		the original value
 * @param val_replace		the replacement value
 * @return					the number of replacements ocurred
 *
 */
native okapi_engine_replace_int(val_search,val_replace)

/**
 *   Replaces every ocurrence of the integer given, in the mod library, with other integer
 *  
 * @param val_search		the original value
 * @param val_replace		the replacement value
 * @return					the number of replacements ocurred
 *
 */
native okapi_mod_replace_int(val_search,val_replace)

/**
 *   Replaces every ocurrence of the float given, in the engine library, with other float
 *  
 * @param val_search		the original value
 * @param val_replace		the replacement value
 * @return					the number of replacements ocurred
 *
 */
native okapi_engine_replace_float(Float:val_search,Float:val_replace)

/**
 *   Replaces every ocurrence of the integer given, in the mod library, with other integer
 *  
 * @param val_search		the original value
 * @param val_replace		the replacement value
 * @return					the number of replacements ocurred
 *
 */
native okapi_mod_replace_float(Float:val_search,Float:val_replace)

/**
 *   Searches for a string in the engine library, starting at 'ptr' address
 *  
 * @param ptr				the start address
 * @param str[]				the string to find
 * @return					the address where the first string was found
 *							0 if none was found
 *
 */
native okapi_engine_ptr_find_string_at(ptr,const str[])

stock okapi_engine_find_string(const str[])
{
	return okapi_engine_ptr_find_string_at(okapi_engine_get_base_ptr(),str)
}

/**
 *   Searches for a string in the mod library, starting at 'ptr' address
 *  
 * @param ptr				the start address
 * @param str[]				the string to find
 * @return					the address where the first string was found
 *							0 if none was found
 *
 */
native okapi_mod_ptr_find_string_at(ptr,const str[])

stock okapi_mod_find_string(const str[])
{
	return okapi_mod_ptr_find_string_at(okapi_mod_get_base_ptr(),str)
}

/**
 *   Searches for an int in the engine library, starting at 'ptr' address
 *  
 * @param ptr				the start address
 * @param val				the value to find
 * @return					the address where the first value was found
 *							0 if none was found
 *
 */
native okapi_engine_ptr_find_int_at(ptr,value)

stock okapi_engine_find_int(value)
{
	return okapi_engine_ptr_find_int_at(okapi_engine_get_base_ptr(),value)
}

/**
 *   Searches for an int in the mod library, starting at 'ptr' address
 *  
 * @param ptr				the start address
 * @param val				the string to find
 * @return					the address where the first value was found
 *							0 if none was found
 *
 */
native okapi_mod_ptr_find_int_at(ptr,val)

stock okapi_mod_find_int(val)
{
	return okapi_mod_ptr_find_int_at(okapi_mod_get_base_ptr(),val)
}

/**
 *   Searches for a float in the engine library, starting at 'ptr' address
 *  
 * @param ptr				the start address
 * @param Float:val			the value to find
 * @return					the address where the first value was found
 *							0 if none was found
 *
 */
native okapi_engine_ptr_find_float_at(ptr,Float:value)

stock okapi_engine_ptr_find_float(Float:value)
{
	return okapi_engine_ptr_find_float_at(okapi_engine_get_base_ptr(),value)
}

/**
 *   Searches for a float in the mod library, starting at 'ptr' address
 *  
 * @param ptr				the start address
 * @param Float:val			the string to find
 * @return					the address where the first value was found
 *							0 if none was found
 *
 */
native okapi_mod_ptr_find_float_at(ptr,Float:val)

stock okapi_mod_find_float(Float:val)
{
	return okapi_mod_ptr_find_float_at(okapi_mod_get_base_ptr(),val)
}

/**
 *   Searches for a byte in the engine library, starting at 'ptr' address
 *  
 * @param ptr				the start address
 * @param val				the value to find
 * @return					the address where the first value was found
 *							0 if none was found
 *
 */
native okapi_engine_ptr_find_byte_at(ptr,value)

stock okapi_engine_find_byte(value)
{
	return okapi_engine_ptr_find_byte_at(okapi_engine_get_base_ptr(),value)
}

/**
 *   Searches for a byte in the mod library, starting at 'ptr' address
 *  
 * @param ptr				the start address
 * @param val				the string to find
 * @return					the address where the first value was found
 *							0 if none was found
 *
 */
native okapi_mod_ptr_find_byte_at(ptr,val)

stock okapi_mod_find_byte(val)
{
	return okapi_mod_ptr_find_byte_at(okapi_mod_get_base_ptr(),val)
}

/**
 *   Searches for an array of bytes in the engine library, starting at 'ptr' address
 *  
 * @param ptr				the start address
 * @param array_[]			the array to find
 * @param count				the number of bytes
 * @return					the address where the first array was found
 *							0 if none was found
 *
 */
native okapi_engine_ptr_find_array_at(ptr,const array_[],count)

stock okapi_engine_find_array(const array_[],count)
{
	return okapi_engine_ptr_find_array_at(okapi_engine_get_base_ptr(),array_,count)
}

/**
 *   Searches for an array of bytes in the mod library, starting at 'ptr' address
 *  
 * @param ptr				the start address
 * @param array_[]			the array to find
 * @param count				the number of bytes
 * @return					the address where the first array was found
 *							0 if none was found
 *
 */
native okapi_mod_ptr_find_array_at(ptr,const array_[],count)

stock okapi_mod_find_array(const array_[],count)
{
	return okapi_mod_ptr_find_array_at(okapi_mod_get_base_ptr(),array_,count)
}

/**
 *   Allocates n blocks of 4 bytes
 *  
 * @param n					number of blocks
 * @return					the address of the allocation (ptr)
 *
 */
native okapi_alloc(n)
