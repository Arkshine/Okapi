
#if defined _okapi_extra_included
	#endinput
#endif
#define _okapi_extra_included

/**
 * Gets the base address of where the provided library is allocated in memory.
 *
 * @note  By default Okapi registers the game as "mod" and the engine as "engine" and
 *        any library where its file path contains "addons". To get the library name, 
 *        module keeps only the file name without extension and without "_i386" for linux.
 *        Examples: "metamod", "amxmodx_mm", "okapi_amxx"
 *
 * @note  Available helpers: okapi_get_mod_address, okapi_get_engine_address
 *
 * @param lib_name          Library name
 *  
 * @return                  Base address on success, 0 otherwise.
 */
native okapi_get_library_address(const lib_name[])

/**
 * Gets the length of the library.
 *
 * @note  By default Okapi registers the game as "mod" and the engine as "engine" and
 *        any library where its file path contains "addons". To get the library name, 
 *        module keeps only the file name without extension and without "_i386" for linux.
 *        Examples: "metamod", "amxmodx_mm", "okapi_amxx"
 *
 * @note  Available helpers: okapi_get_mod_size, okapi_get_engine_size
 *
 * @param lib_name          Library name
 *  
 * @return                  Base address on success, 0 otherwise.
 */
native okapi_get_library_size(const lib_name[])

/**
 * Finds informations of a library from a given address.
 * This will look up among all libraries registered by module.
 *
 * @param ptr               Address to search
 * @param lib_ptr           Variable to store the library base address in
 * @param lib_size          Variable to store the library length in
 * @param lib_name          Buffer to copy library name to
 * @param lib_namelen       Maximum size of the buffer
 *
 * @return                  true on successful lookup, false otherwise.
 */
native bool:okapi_find_library_by_ptr(ptr, &lib_ptr, &lib_size = 0, lib_name[] = "", lib_namelen = 0);

/**
 *  Converts a cbase (that is, the address of the c++ object of an entity) to an id
 *  
 * @param cbase				the cbase of the entity
 * @return					the id of the entity
 *
 *
 */
native okapi_cbase_to_id(cbase)

/**
 *  Converts the id of an entity to its cbase (that is, the address of the c++ object of an entity) 
 *  
 * @param id				the id of the entity
 * @return					the cbase of the entity
 *
 *
 */
native okapi_id_to_cbase(id)

/**
 *  Retrieves a virtual function address located in the virtual table of an entity
 *  
 * @param id				the id of the entity
 * @param offset			the offset of the virtual function in the virtual table
 * @return					the address of the virtual function
 *
 *
 */
native okapi_cbase_get_vfunc_ptr(id,offset)

/**
 *  Retrieves a virtual function address located in the virtual table of an entity, created using it's classname
 *  
 * @param classname			the classname of the entity
 * @param offset			the offset of the virtual function in the virtual table
 * @return					the address of the virtual function
 *
 *
 */
native okapi_class_get_vfunc_ptr(classname[],offset)

/**
 *  Retrieves a virtual function address located in the virtual table of an object (can be a cbase or another one)
 *  
 * @param ptr				the address of the object
 * @param offset			the offset of the virtual function in the virtual table
 * @return					the address of the virtual function
 *
 *
 */
native okapi_ptr_get_vfunc_ptr(ptr,offset)


/**
 * Retrieves the memory protection of the location pointed to by the address
 *  
 * @param ptr			the address
 * @return				the protection value
 *
 */
native okapi_get_mem_protect(ptr)

/**
 * Changes the memory protection of the location pointed to by the address
 *  
 * @param ptr			the address
 * @param prot			the protection value
 * @return				if the operation was successful
 *
 */
native okapi_set_mem_protect(ptr,prot)


/**
 *  Retrieves the symbolic name of an address, if one exists
 *	This functions just works/makes sense on linux
 *  
 * @param ptr				the address of the function
 * @param str[]				the string to save the name
 * @param len				the max length that the string will hold
 * @return					1 or 0, depending on the sucess of the operation
 *
 *
 */
native okapi_get_ptr_symbol(ptr,str[],len)

/**
 *  Retrieves the address of an engfunc function
 *  
 * @param id				the id of the engfunc (a value from the enum defined in fakemeta_const like EngFunc_PrecacheModel)
 * @return					the address of the function
 *
 *
 */
native okapi_get_engfunc_ptr(id)

/**
 *  Retrieves the address of an dllfunc function
 *  
 * @param id				the id of the dllfunc (a value from the enum defined in fakemeta_const like DLLFunc_GameInit)
 * @return					the address of the function
 *
 *
 */
native okapi_get_dllfunc_ptr(id)

/**
 *  Retrieves the address of an engfunc function by its relative offset in the struct enginefuncs_t
 *  
 * @param offset			the offset of the engfunc
 * @return					the address of the function
 *
 *
 */
native okapi_get_engfunc_ptr_by_offset(offset)

/**
 *  Retrieves the address of a dllfunc function by its relative offset in the struct DLL_FUNCTIONS
 *  
* @param offset				the offset of the dllfunc
* @return					the address of the function
 *
 *
 */
native okapi_get_dllfunc_ptr_by_offset(offset)


// To keep backward compatibility
// -
#define okapi_get_base_ptr  okapi_get_library_address

