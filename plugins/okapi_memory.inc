//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// Okapi Memory Include
//

#if defined _okapi_memory_included
	#endinput
#endif
#define _okapi_memory_included

/**
 * Searches for a signature in the mod library in a given range and starting address.
 *
 * @note  For bytes to ignore, use "*" or any number above 0xFF like:
 *          {0x51,0x56,"*","*",0x8B,0x86}.
 *          {0x51,0x56,0xDEF,0xDEF,0x8B,0x86}.
 *
 * @note Available helpers:
 *          okapi_mod_find_sig_at  okapi_engine_find_sig_at
 *          okapi_mod_find_sig     okapi_engine_find_sig
 *
 * @param start_address     Address where the search starts
 * @param range_length      Range length of bytes where function is allowed to search in.
 * @param signature[]       Signature to find
 * @param size              Size of the signature
 *
 * @return                  Address where the sig was first found, 0 otherwise.
 */
native okapi_find_sig(start_address, range_length, const signature[], count = sizeof signature);

/**
 * Gets the address of a function located in a registered library, given it's symbolic name.
 *
 * @note Available helpers: okapi_mod_find_symbol, okapi_engine_find_symbol
 *
 * @param lib_ptr           Library base address to search function address in.
 * @param symbol            Symbolic name of a function
 *
 * @return                  Address of the function on success, 0 otherwise
 */
native okapi_find_symbol(lib_ptr, const symbol[]);

/**
 * Retrieves the value in an address.
 *
 * @param address           The address where is located the value
 * @param type              The value data type, See okapi_mem_* constants
 * @param ...               If zero additional parameters are provided, the function
 *                          will return an integer or float value directly.
 *                          If one additional parameter is provided, the function will
 *                          store an integer as float, a float or vector.
 *                          If two additional parameters are provided, the function
 *                          will copy a string or an array to the buffer provided
 *                          in the second parameter, using the third as the maximum
 *                          buffer size.
 *
 * @return                  If zero additional parameters are provided, the function
 *                          will return an integer or float value.
 *                          If two additional parameters are provided and data typ is
 *                          a string, the function will return the number of cells
 *                          written to the buffer.
 *
 * @error                   Invalid return type
 *                          Incorrect parameters count
 */
native any:okapi_mem_get(address, type, any:...);

/**
 * Sets the value of the location pointed by the address.
 *
 * @param address           The address where is located the value
 * @param type              The value data type, See okapi_mem_* constants
 * @param ...               The value to set. If parameter is an array,
 *                          its size is required as second parameter.
 * @noreturn
 * @error                   Incorrect parameters count
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
 * @note Available helpers:
 *       okapi_mod_replace_string()    okapi_engine_replace_string()
 *       okapi_mod_replace_array()     okapi_engine_replace_array()
 *       okapi_mod_replace_int()       okapi_engine_replace_int()
 *       okapi_mod_replace_float()     okapi_engine_replace_float()
 *
 * @param start_address     Address where the search starts
 * @param range_length      Length range of bytes where function is allowed to search in.
 * @param type              Value data type, See okapi_mem_* constants
 * @param ...               Variable parameters to provide.
 *                          if type is integer or float: <old_value> <new_value>
 *                          if type is array : <old array> <new_array> <num_bytes>
 *                          if type is string: <old string> <new_string> [force = 0]
 *                          If force parameter is set to 1, it will skip the
 *                          restrition of the string replacement size.
 *
 * @return                  Number of replacements ocurred
 *
 * @error                   Invalid data type
 *                          Incorrect parameters count
 *                          Invalid replacement string length
 *                          Missing array size
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
 * @param start_address     Address where the search starts
 * @param range_length      Length range of bytes where function is allowed to search in.
 * @param type              Value data type, See okapi_mem_* constants
 * @param ...               Variable parameters to provide.
 *                          if type is intege, float, string: <value>
 *                          if type is array : <array> <size_array>ze.
 *
 * @return                  Address where the first string was found, 0 otherwise.
 *
 * @error                   Invalid data type
 *                          Incorrect parameters count
 *                          Missing array size parameter
 */
native okapi_mem_find(start_address, range_length, type, any:...);

/**
 * Retrieves the memory protection of the location pointed to by the address.
 *
 * @param ptr           Address in memory
 *
 * @return              Protection value, see PAGE_* constants
 */
native okapi_get_mem_protect(ptr);

/**
 * Changes the memory protection of the location pointed to by the address
 *
 * @param ptr           Address in memory
 * @param prot          Protection value, see see PAGE_* constants
 *
 * @return              true if the operation was successful, false otherwise
 */
native bool:okapi_set_mem_protect(ptr, prot);

/**
 * Allocates n blocks of 4 bytes
 *
 * @param n                 number of blocks
 * @return                  the address of the allocation (ptr)
 */
native okapi_alloc(n)
