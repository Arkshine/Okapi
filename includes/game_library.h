//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// Okapi Module
//

#ifndef GAME_LIBRARY_H
#define GAME_LIBRARY_H

#include "library_utils.h"

class GameLibrary : public s_library
{
	public:

		virtual const char* get_name() = 0;

		GameLibrary(s_library s);

		const char* get_address_sym (void* address);
		int         find_func       (const char* function_name);
		bool        contains_address(long address);
};

class GameLibraryEngine : public GameLibrary
{
	public:

		GameLibraryEngine(s_library s) : GameLibrary(s) {};

		const char* get_name();
};

class GameLibraryMod : public GameLibrary
{
	public:

		GameLibraryMod(s_library s) : GameLibrary(s) {};

		const char* get_name();
};

#endif // GAME_LIBRARY_H
