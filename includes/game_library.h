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

#include <s_library.h>
#include <am-vector.h>

class GameLibrary : public s_library
{
	protected:

		void init()
		{
		}

	public:

		virtual const char* get_name() = 0;

		GameLibrary(s_library s)
		{
			this->address = s.address;
			this->length  = s.length;
			this->handle  = s.handle;
		}

		const char* get_address_sym(void* address)
		{
			return get_address_symbol((void*)((int)this->address + (int)address));
		}

		int find_func(const char* function_name)
		{
			int address = (int)find_function(this, function_name);

			if (!address)
			{
				return 0;
			}

			return address - (int)this->address;
		}

		bool contains_address(long address)
		{
			return (address >= (long)this->address) && (address < ((long)this->address + this->length));
		}
};

class GameLibraryEngine : public GameLibrary
{
	public:

		GameLibraryEngine(s_library s) : GameLibrary(s)
		{
			this->init();
		}

		const char* get_name()
		{
			return "engine";
		}
};

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

#endif // GAME_LIBRARY_H
