
#if defined _okapi_bcompat_included
	#endinput
#endif
#define _okapi_bcompat_included

#include <okapi_const>
#include <okapi_memory>

/**
 * Sets the value of the location pointed by the address with the entvars relative to an entity represented by id
 *  
 * @param ptr			the address
 * @param id			the entity id
 *
 */
stock okapi_set_ptr_ent(ptr, id) 
{
	return okapi_mem_get(ptr, mem_type_entvars, id); 
}

/**
 * Retrieves the entvars located in an address as an entity id
 *  
 * @param ptr			the address
 * @return				the entity id
 *
 */
stock okapi_get_ptr_ent(ptr)
{
	return okapi_mem_get(ptr, mem_type_entvars);
}

/**
 * Sets the value of the location pointed by the address with the edict relative to an entity represented by id
 *  
 * @param ptr			the address
 * @param id			the entity id
 *
 */
stock okapi_set_ptr_edict(ptr,id)
{
	return okapi_mem_set(ptr, mem_type_edict, id);
}

/**
 * Retrieves the edict located in an address as an entity id
 *  
 * @param ptr			the address
 * @return				the entity id
 *
 */
stock okapi_get_ptr_edict(ptr)
{
	return okapi_mem_get(ptr, mem_type_edict);
}

/**
 * Sets the value of the location pointed by the address with the cbase relative to an entity represented by id
 *  
 * @param ptr			the address
 * @param id			the entity id
 *
 */
stock okapi_set_ptr_cbase(ptr,id)
{
	return okapi_mem_set(ptr, mem_type_cbase, id);
}

/**
 * Retrieves the cbase located in an address as an entity id
 *  
 * @param ptr			the address
 * @return				the entity id
 *
 */
stock okapi_get_ptr_cbase(ptr)
{
	return okapi_mem_get(ptr, mem_type_cbase);
}

/**
 * Writes an integer value at the location pointed by the address
 *  
 * @param ptr			the address
 * @param value			the value
 *
 */
stock okapi_set_ptr_int(ptr, value)
{
	return okapi_mem_set(ptr, mem_type_int, value);
}

/**
 * Retrieves an integer value in the location pointed by the address
 *  
 * @param ptr			the address
 * @return				the value
 *
 */
stock okapi_get_ptr_int(ptr)
{
	return okapi_mem_get(ptr, mem_type_int);
}

/**
 * Writes a byte value at the location pointed by the address
 *  
 * @param ptr			the address
 * @param value			the value
 *
 */
stock okapi_set_ptr_byte(ptr, value)
{
	return okapi_mem_set(ptr, mem_type_byte, value);
}

/**
 * Retrieves a byte value in the location pointed by the address
 *  
 * @param ptr			the address
 * @return				the value
 *
 */
stock okapi_get_ptr_byte(ptr)
{
	return okapi_mem_get(ptr, mem_type_byte);
}

/**
 * Writes a float value at the location pointed by the address
 *  
 * @param ptr			the address
 * @param value			the value
 *
 */
stock okapi_set_ptr_float(ptr, Float:value)
{
	return okapi_mem_set(ptr, mem_type_float, value);
}

/**
 * Retrieves a float value in the location pointed to by the address
 *  
 * @param ptr			the address
 * @return				the value
 *
 */
stock Float:okapi_get_ptr_float(ptr)
{
	return okapi_mem_get(ptr, mem_type_float);
}

/**
* Writes an array of bytes to the location pointed to by the address
*
* @param ptr			the address
* @param holder		the array with the bytes to write
* @param count			the number of bytes to write
*
*/
stock okapi_set_ptr_array(ptr, holder[], count)
{
	return okapi_mem_set(ptr, mem_type_array, holder, count);
}

/**
 * Retrieves an array of bytes in the location pointed to by the address
 *  
 * @param ptr			the address
 * @param holder		the array where the bytes will be stored
 * @param count			the number of bytes to retrieve
 *
 */
stock okapi_get_ptr_array(ptr, holder[], count)
{
	return okapi_mem_get(ptr, mem_type_array, holder, count);
}

/**
* Writes a vector to the location pointed to by the address
*
* @param ptr			the address
* @param vec			the variable where the vector will be retrieved from
*
*/
stock okapi_set_ptr_vec(ptr, Float:vec[3])
{
	return okapi_mem_set(ptr, mem_type_vector, vec);
}

/**
* Retrieves an array of bytes in the location pointed to by the address
*
* @param ptr			the address
* @param vec		the variable where the vector will be wrote to
*/
stock okapi_get_ptr_vec(ptr, Float:vec[3])
{
	return okapi_mem_get(ptr, mem_type_vector, vec);
}

/**
 * Writes a string to the location pointed to by the address
 *
 * @param ptr			the address
 * @param str[]			the string to write
 *
 */
stock okapi_set_ptr_string(ptr, str[])
{
	return okapi_mem_set(ptr, mem_type_string, str);
}

/**
 * Retrieves an string from the location pointed to by the address
 *  
 * @param ptr			the address
 * @param str[]			the variable where the string will be stored
 * @param len			the limit of chars to retrieve 
 * @return				the number of chars retrieved
 *
 */
stock okapi_get_ptr_string(ptr, str[], len)
{
	return okapi_mem_get(ptr, mem_type_string, str, len);
}
