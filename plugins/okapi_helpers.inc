
#if defined _okapi_helpers_included
	#endinput
#endif
#define _okapi_helpers_included

#include <okapi_extra>
#include <okapi_memory>


// To get library address
// ----------------------
//      okapi_get_mod_address()
//      okapi_get_engine_address()
//
// To get library length
// ---------------------
//      okapi_get_mod_size()
//      okapi_get_engine_size()
//
// To replace value in memory
// --------------------------
//      okapi_mod_replace_string()
//      okapi_mod_replace_array()
//      okapi_mod_replace_int()
//      okapi_mod_replace_float()
//      okapi_engine_replace_string()
//      okapi_engine_replace_array()
//      okapi_engine_replace_int()
//      okapi_engine_replace_float()


// To get library address
// ----------------------

	/**
	 * Gets the base address of where the mod library is allocated in memory.
	 *
	 * @return          Base address on success, 0 otherwise.
	 *
	 */
	stock okapi_get_mod_address()
	{
		static address;

		if (!address)
		{
			address = okapi_get_library_address("mod");
		}

		return address;
	}

	/**
	 * Gets the base address of where the engine library is allocated in memory.
	 *
	 * @return          Base address on success, 0 otherwise.
	 */
	stock okapi_get_engine_address()
	{
		static address;

		if (!address)
		{
			address = okapi_get_library_address("engine");
		}

		return address;
	}


// To get library length
// ---------------------

	/**
	 * Gets the length of the mod library
	 *
	 * @return          Number of bytes on succes, otherwise 0.
	 */
	stock okapi_get_mod_size()
	{
		static size;

		if (!size)
		{
			size = okapi_get_library_size("mod");
		}

		return size;
	}

	/**
	 * Gets the length of the mod library.
	 *
	 * @return          Number of bytes on succes, otherwise 0.
	 */
	stock okapi_get_engine_size()
	{
		static size;

		if (!size)
		{
			size = okapi_get_library_size("engine");
		}

		return size;
	}


// To replace value in memory
// --------------------------

	/**
	 * Replaces every string that occurs in the mod library with another one.
	 *
	 * @note The replacement string should be of equal or shorter size than the original.
	 *       If you know what you are doing and want to skip this check, make force to 1.
	 *
	 * @param str_orig[]        Original string
	 * @param str_replace[]     Replacement string
	 * @param force             Setting to 1 will skip the restrition of the string replacement size
	 *
	 * @return                  Number of replacements ocurred on success, 0 otherwise.
	 * @error                   Invalid replacement string length
	 */
	stock okapi_mod_replace_string(const str_orig[], const str_replace[], force = 0)
	{
		return okapi_mem_replace(okapi_get_mod_address(), okapi_get_mod_size(), okapi_mem_string, str_orig, str_replace, force);
	}

	/**
	 * Replaces every ocurrence of the sequence of bytes given, in the mod library, with other sequence.
	 *
	 * @param arr_orig[]        Original array
	 * @param arr_replace[]     Replacement array
	 * @param arr_len           Number of bytes to replace
	 *
	 * @return                  Number of replacements ocurred on success, 0 otherwise.
	 */
	stock okapi_mod_replace_array(any:arr_orig[], any:arr_replace[], arr_len)
	{
		return okapi_mem_replace(okapi_get_mod_address(), okapi_get_mod_size(), okapi_mem_array, arr_orig, arr_replace, arr_len);
	}

	/**
	 * Replaces every ocurrence of the integer given, in the mod library, with other integer.
	 *
	 * @param val_search        Original value
	 * @param val_replace       Replacement value
	 *
	 * @return                  Number of replacements ocurred on success, 0 otherwise.
	 */
	stock okapi_mod_replace_int(any:val_search, any:val_replace)
	{
		return okapi_mem_replace(okapi_get_mod_address(), okapi_get_mod_size(), okapi_mem_int, val_search, val_replace);
	}

	/**
	 * Replaces every ocurrence of the float given, in the mod library, with other float.
	 *
	 * @param val_search        Original value
	 * @param val_replace       Replacement value
	 *
	 * @return                  Number of replacements ocurred on success, 0 otherwise.
	 */
	stock okapi_mod_replace_float(Float:val_search,Float:val_replace)
	{
		return okapi_mem_replace(okapi_get_mod_address(), okapi_get_mod_size(), okapi_mem_float, val_search, val_replace);
	}

	/**
	 * Replaces every string that occurs in the engine library with another one.
	 *
	 * @note The replacement string should be of equal or shorter size than the original.
	 *       If you know what you are doing and want to skip this check, make force to 1.
	 *
	 * @param str_orig[]        Original string
	 * @param str_replace[]     Replacement string
	 * @param force             Setting to 1 will skip the restrition of the string replacement size
	 *
	 * @return                  Number of replacements ocurred on success, 0 otherwise.
	 * @error                   Invalid replacement string length
	 */
	stock okapi_engine_replace_string(const str_orig[], const str_replace[], force = 0)
	{
		return okapi_mem_replace(okapi_get_engine_address(), okapi_get_engine_size(), okapi_mem_string, str_orig, str_replace, force);
	}

	/**
	 * Replaces every ocurrence of the sequence of bytes given, in the engine library, with other sequence.
	 *
	 * @param arr_orig[]        Original array
	 * @param arr_replace[]     Replacement array
	 * @param arr_len           Number of bytes to replace
	 *
	 * @return                  Number of replacements ocurred on success, 0 otherwise.
	 */
	stock okapi_engine_replace_array(any:arr_orig[], any:arr_replace[], arr_len)
	{
		return okapi_mem_replace(okapi_get_engine_address(), okapi_get_engine_size(), okapi_mem_array, str_orig, str_replace, arr_len);
	}

	/**
	 * Replaces every ocurrence of the integer given, in the engine library, with other integer.
	 *
	 * @param val_search        Original value
	 * @param val_replace       Replacement value
	 *
	 * @return                  Number of replacements ocurred on success, 0 otherwise.
	 */
	stock okapi_engine_replace_int(any:val_search, any:val_replace)
	{
		return okapi_mem_replace(okapi_get_engine_address(), okapi_get_engine_size(), okapi_mem_int, val_search, val_replace);
	}

	/**
	 * Replaces every ocurrence of the float given, in the engine library, with other float.
	 *
	 * @param val_search        Original value
	 * @param val_replace       Replacement value
	 *
	 * @return                  Number of replacements ocurred on success, 0 otherwise.
	 */
	stock okapi_engine_replace_float(Float:val_search,Float:val_replace)
	{
		return okapi_mem_replace(okapi_get_engine_address(), okapi_get_engine_size(), okapi_mem_float, val_search, val_replace);
	}


// To keep backward compatibility
// ------------------------------

	#define okapi_mod_get_size         okapi_get_mod_size
	#define okapi_engine_get_size      okapi_get_engine_size

	#define okapi_mod_get_base_ptr     okapi_get_mod_address
	#define okapi_engine_get_base_ptr  okapi_get_engine_address
