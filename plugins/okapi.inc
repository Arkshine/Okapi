//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// Okapi Main Include
//

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
#include <okapi_helpers>
#include <okapi_extra>

/**
 * Attaches okapi to a method (class member function) so you can hook it and call it.
 *
 * @param ptr               Address of the method
 * @param ret               Return type of the method
 * @param class             Type of the method class
 * @param ...               Rest of the types for the arguments of the function
 *
 * @return                  Handler to the method attached
 * @error                   Invalid method address
 */
native okapi_func:okapi_build_method(ptr, okapi_arg:ret, okapi_arg:class, okapi_arg:...);

/**
 * Attaches okapi to a function so you can hook it and call it.
 *
 * @param ptr               Address of the method
 * @param ret               Return type of the method
 * @param ...               Rest of the types for the arguments of the function
 *
 * @return                  Handler to the function attached
 * @error                   Invalid function address
 */
native okapi_func:okapi_build_function(ptr, okapi_arg:ret, okapi_arg:...);

/**
 * Attaches okapi to a virtual function of an entity so you can hook it and call it.
 *
 * @note  You don't need to pass arg_cbase in the argument list for the entity itself.
 *
 * @param id                Index of the entity
 * @param offset            Offset of the function in the virtual table
 * @param ret               Return type of the method
 * @param ...               Rest of the types for the arguments of the function
 *
 * @return                  Handler to the function attached
 * @error                   Invalid entity
 */
native okapi_func:okapi_build_vfunc_cbase(id, offset, okapi_arg:ret, okapi_arg:...);

/**
 * Attaches okapi to a virtual function of an entity (using its class) so you can hook it and call it.
 *
 * @note  You don't need to pass arg_cbase in the argument list for the entity itself.
 *
 * @param classname         Name of the class
 * @param offset            Offset of the function in the virtual table
 * @param ret               Return type of the method
 * @param ...               Rest of the types for the arguments of the function
 *
 * @return                  Handler to the function attached
 * @error                   Invalid class name
 */
native okapi_func:okapi_build_vfunc_class(const classname[], offset, okapi_arg:ret, okapi_arg:...);

/**
 * Attaches okapi to a virtual function of an object so you can hook it and call it.
 *
 * @note  You don't need to pass arg_cbase in the argument list for the entity itself.
 *
 * @param ptr               Address of the object
 * @param offset            Offset of the function in the virtual table
 * @param ret               Return type of the method
 * @param ...               Rest of the types for the arguments of the function
 *
 * @return                  Handler to the function attached
 * @error                   Invalid object address
 */
native okapi_func:okapi_build_vfunc_ptr(ptr,offset,okapi_arg:ret,okapi_arg:...)


/**
 * Adds a hook to a previously attached function.
 *
 * @param func              Handler to the function returned by an attach function
 * @param callback          Nname of the public function in the code that will be
 *
 * @return                  Handler to the hook
 * @error                   Inexistant callback
 */
native okapi_hook:okapi_add_hook(okapi_func:func, const callback[], post = 0);

/**
 * Removes a hook from the function.
 *
 * @param hook              the handler to the hook
 *
 * @noreturn
 * @error                   Invalid hook
 */
native okapi_del_hook(&okapi_hook:hook);

/**
 * Removes current hook from the function.
 * This is meant to be used inside a hook (when you want to remove it there).
 *
 * @noreturn
 * @error                   Invalid hook
 */
native okapi_del_current_hook();

/**
 * Modifies a parameter that will be passed in the call to the original function.
 *
 * @param param_id          Index of the parameter. First is 1.
 * @param ...               Value of the parameter
 *
 * @noreturn
 * @error                   Not inside a hook
 *                          Invalid parameter count
 *                          Invalid parameter index
 */
native okapi_set_param(param_id, any:...);

/**
 * Modifies the return of the function.
 *
 * @param ...               Value of the return
 *
 * @noreturn
 * @error                   Not inside a hook
 *                          Set return only available in pre hook
 *                          Invalid parameter count
 */
native okapi_set_return(any:...);

/**
 * Retrieves the value that the hooked function retrieved.
 *
 * @param ...               Value of the original return, if passed by reference
 *
 * @return                  Value of the original return, if passed by value
 *
 * @error                   Not inside a hook
 *                          Get return only available in post hook
 *                          Invalid parameter count
 */
native okapi_get_orig_return(any:...);

/**
 * Calls a function without calling its hooks.
 *
 * @param ...               Arguments to the function and return variables, if the function returns by reference
 *
 * @return                  Value that the function returned, if the function doesn't return by reference
 * @error                   Invalid parameter count
 */
native okapi_call(okapi_func:func, any:...);

/**
 * Calls a function and its hooks
 *
 * @param ...               Arguments to the function and return variables, if the function returns by reference
 *
 * @return                  Value that the function returned, if the function doesn't return by reference
 * @error                   Invalid parameter count
 */
native okapi_call_ex(okapi_func:func, any:...);

/**
 * Gets the hook that is currently being called.
 * This is meant to be used inside a hook (when you want to remove it there).
 *
 * @return                  Handler to the hook
 * @error                   Not inside a hook
 */
native okapi_hook:okapi_current_hook();

/**
 * Returns a if server is windows, returns b if server is linux.
 *
 * @param a                 Value for windows
 * @param b                 Value for linux
 *
 * @return                  a if windows, b if linux
 */
native wl(a, b);
