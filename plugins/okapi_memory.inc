
#if defined _okapi_memory_included
	#endinput
#endif
#define _okapi_memory_included

/**
 * Sets the value of the location pointed by the address with the entvars relative to an entity represented by id
 *  
 * @param ptr			the address
 * @param id			the entity id
 *
 */
native okapi_set_ptr_ent(ptr,id)

/**
 * Retrieves the entvars located in an address as an entity id
 *  
 * @param ptr			the address
 * @return				the entity id
 *
 */
native okapi_get_ptr_ent(ptr)

/**
 * Sets the value of the location pointed by the address with the edict relative to an entity represented by id
 *  
 * @param ptr			the address
 * @param id			the entity id
 *
 */
native okapi_set_ptr_edict(ptr,id)

/**
 * Retrieves the edict located in an address as an entity id
 *  
 * @param ptr			the address
 * @return				the entity id
 *
 */
native okapi_get_ptr_edict(ptr)

/**
 * Sets the value of the location pointed by the address with the cbase relative to an entity represented by id
 *  
 * @param ptr			the address
 * @param id			the entity id
 *
 */
native okapi_set_ptr_cbase(ptr,id)

/**
 * Retrieves the cbase located in an address as an entity id
 *  
 * @param ptr			the address
 * @return				the entity id
 *
 */
native okapi_get_ptr_cbase(ptr)

/**
 * Writes an integer value at the location pointed by the address
 *  
 * @param ptr			the address
 * @param value			the value
 *
 */
native okapi_set_ptr_int(ptr,value)

/**
 * Retrieves an integer value in the location pointed by the address
 *  
 * @param ptr			the address
 * @return				the value
 *
 */
native okapi_get_ptr_int(ptr)

/**
 * Writes a byte value at the location pointed by the address
 *  
 * @param ptr			the address
 * @param value			the value
 *
 */
native okapi_set_ptr_byte(ptr,value)

/**
 * Retrieves a byte value in the location pointed by the address
 *  
 * @param ptr			the address
 * @return				the value
 *
 */
native okapi_get_ptr_byte(ptr)

/**
 * Writes a float value at the location pointed by the address
 *  
 * @param ptr			the address
 * @param value			the value
 *
 */
native okapi_set_ptr_float(ptr,Float:value)

/**
 * Retrieves a float value in the location pointed to by the address
 *  
 * @param ptr			the address
 * @return				the value
 *
 */
native Float:okapi_get_ptr_float(ptr)

/**
 * Retrieves an array of bytes in the location pointed to by the address
 *  
 * @param ptr			the address
 * @param holder		the array where the bytes will be stored
 * @param count			the number of bytes to retrieve
 *
 */
native okapi_get_ptr_array(ptr,holder[],count)

/**
 * Writes an array of bytes to the location pointed to by the address
 *  
 * @param ptr			the address
 * @param holder		the array with the bytes to write
 * @param count			the number of bytes to write
 *
 */
native okapi_set_ptr_array(ptr,holder[],count)

/**
 * Retrieves an string from the location pointed to by the address
 *  
 * @param ptr			the address
 * @param str[]			the variable where the string will be stored
 * @param len			the limit of chars to retrieve 
 * @return				the number of chars retrieved
 *
 */
native okapi_get_ptr_string(ptr,str[],len)

/**
 * Writes a string to the location pointed to by the address
 *  
 * @param ptr			the address
 * @param str[]			the string to write
 *
 */
native okapi_set_ptr_string(ptr,str[])

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

/**
 *   Reads a vector from 
 *  
 * @param ptr				the ptr of the allocation
 * @param Float:vec[3]		the variable where the vector will be wrote to
 *
 *
 */
native okapi_get_ptr_vec(ptr,Float:vec[3])