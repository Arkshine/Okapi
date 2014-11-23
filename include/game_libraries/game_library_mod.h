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

#ifndef __GAME_LIBRARY_MOD_H__
#define __GAME_LIBRARY_MOD_H__

#include <game_library.h>

class GameLibraryMod : public GameLibrary
{
	public:

		GameLibraryMod(s_library s) : GameLibrary(s)
		{
			this->init();
		}

		const char* get_name()
		{
			return "mod";
		}
};

#endif // __GAME_LIBRARY_MOD_H__

