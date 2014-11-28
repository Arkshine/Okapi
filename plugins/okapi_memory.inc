
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
 * Replaces every value with another one that occurs in a range from a start address.
 * 
 * @note Only string, array, integer and float can be replaced.
 *
 * @note The replacement string should be of equal or shorter size than the original.
 *       If you know what you are doing and want to skip this check, make force to 1.
 *
 * @note Available helpers where you don't need to provide address, range length and type: 
 *       okapi_mod_replace_string()    okapi_engine_replace_string()
 *       okapi_mod_replace_array()     okapi_engine_replace_array()
 *       okapi_mod_replace_int()       okapi_engine_replace_int()
 *       okapi_mod_replace_float()     okapi_engine_replace_float()
 *
 * @param start_address Address where the search starts
 * @param range_length  Length range of bytes where function is allowed to search in.
 * @param type			Value data type, See okapi_mem_* constants
 * @param ...			Variable parameters to provide.
 *                      if type is integer or float: <old_value> <new_value>
 *                      if type is array : <old array> <new_array> <num_bytes>
 *                      if type is string: <old string> <new_string> [force = 0]
 *                        If force parameter is set to 1, it will skip the 
 *                        restrition of the string replacement size.
 *
 * @return				Number of replacements ocurred
 *
 * @error				Invalid data type
 *						Incorrect parameters count
 *                      Invalid replacement string length
 *						Missing array size
 */
native okapi_mem_replace(start_address, range_length, type, any:...);

/**
 * Searches for a value in a library in a range from a start address.
 * 
 * @note Only string, array, integer and float can be searched.
 * @note Available helpers:
 *       okapi_mod_find_string_at      okapi_engine_find_string_at
 *       okapi_mod_find_string         okapi_engine_find_string
 *       okapi_mod_find_array_at       okapi_engine_find_array_at
 *       okapi_mod_find_array          okapi_engine_find_array
 *       okapi_mod_find_int_at         okapi_engine_find_int_at
 *       okapi_mod_find_int            okapi_engine_find_int
 *       okapi_mod_find_float_at       okapi_engine_find_float_at
 *       okapi_mod_find_float          okapi_engine_find_float
 *       okapi_mod_find_byte_at        okapi_engine_find_byte_at
 *       okapi_mod_find_byte           okapi_engine_find_byte
 *
 * @param start_address Address where the search starts
 * @param range_length  Length range of bytes where function is allowed to search in.
 * @param type			Value data type, See okapi_mem_* constants
 * @param ...			Variable parameters to provide.
 *                      if type is intege, float, string: <value>
 *                      if type is array : <array> <size_array>ze.
 *
 * @return				Address where the first string was found, 0 otherwise.
 *
 * @error				Invalid data type
 *						Incorrect parameters count
 *						Missing array size parameter
 */
native okapi_mem_find(start_address, range_length, type, any:...);

/**
 *   Allocates n blocks of 4 bytes
 *  
 * @param n					number of blocks
 * @return					the address of the allocation (ptr)
 *
 */
native okapi_alloc(n)
