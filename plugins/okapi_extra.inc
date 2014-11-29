//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// Okapi Extra Include
//

#if defined _okapi_extra_included
	#endinput
#endif
#define _okapi_extra_included

/**
 * Gets the base address of where the provided library is allocated in memory.
 *
 * @note  By default Okapi registers the game as "mod", the engine as "engine" and
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
native okapi_get_library_ptr(const lib_name[])

/**
 * Gets the length of the library.
 *
 * @note  By default Okapi registers the game as "mod", the engine as "engine" and
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
native bool:okapi_find_library_by_ptr(ptr, &lib_ptr, &lib_size = 0, const lib_name[] = "", lib_namelen = 0);

/**
 * Converts a cbase (that is, the address of the c++ object of an entity) to an id.
 *
 * @param cbase             Cbase of the entity
 *
 * @return                  Index of the entity
 */
native okapi_cbase_to_id(cbase);

/**
 * Converts the id of an entity to its cbase (that is, the address of the c++ object of an entity).
 *
 * @param id                Index of the entity
 *
 * @return                  Cbase of the entity
 */
native okapi_id_to_cbase(id);

/**
 * Retrieves a virtual function address located in the virtual table of an entity.
 *
 * @param id                Index of the entity
 * @param offset            Offset of the virtual function in the virtual table
 *
 * @return                  Address of the virtual function on sucess, 0 otherwise.
 */
native okapi_cbase_get_vfunc_ptr(id, offset);

/**
 * Retrieves a virtual function address located in the virtual table of an entity, created using it's classname.
 *
 * @param classname         Classname of the entity
 * @param offset            Offset of the virtual function in the virtual table
 *
 * @return                  Address of the virtual function
 *
 * @error                   Invalid class name
 */
native okapi_class_get_vfunc_ptr(const classname[], offset);

/**
 * Retrieves a virtual function address located in the virtual table of an object (can be a cbase or another one).
 *
 * @param ptr               Address of the object
 * @param offset            Offset of the virtual function in the virtual table
 *
 * @return                  Address of the virtual function
 */
native okapi_ptr_get_vfunc_ptr(ptr, offset);

/**
 * Retrieves the symbolic name of an address, if one exists.
 * This functions just works/makes sense on linux/osx.
 *
 * @param ptr               Address of the function
 * @param str[]             String to save the name
 * @param len               Max length that the string will hold
 *
 * @return                  true on succes or false otherwise
 */
native bool:okapi_get_ptr_symbol(ptr, str[], len);

/**
 * Retrieves the address of an engfunc function.
 *
 * @param id                Index of the engfunc (a value from the enum defined in fakemeta_const like EngFunc_PrecacheModel)
 * @return                  Address of the function
 */
native okapi_get_engfunc_ptr(id);

/**
 * Retrieves the address of an dllfunc function.
 *
 * @param id                Index of the dllfunc (a value from the enum defined in fakemeta_const like DLLFunc_GameInit)
 * @return                  Address of the function
 */
native okapi_get_dllfunc_ptr(id);

/**
 * Retrieves the address of an engfunc function by its relative offset in the struct enginefuncs_t.
 *
 * @param offset            Offset of the engfunc
 * @return                  Address of the function
 */
native okapi_get_engfunc_ptr_by_offset(offset);

/**
 * Retrieves the address of a dllfunc function by its relative offset in the struct DLL_FUNCTIONS.
 *
 * @param offset            Offset of the dllfunc
 * @return                  Address of the function
 */
native okapi_get_dllfunc_ptr_by_offset(offset);


// To keep backward compatibility
// -
#define okapi_get_base_ptr  okapi_get_library_ptr

