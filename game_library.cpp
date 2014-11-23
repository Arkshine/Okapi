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

#include "game_library.h"

GameLibrary::GameLibrary(s_library s)
{
	this->address = s.address;
	this->length  = s.length;
	this->handle  = s.handle;
}

const char* GameLibrary::get_address_sym(void* address)
{
	return get_address_symbol((void*)((int)this->address + (int)address));
}

int GameLibrary::find_func(const char* function_name)
{
	int address = (int)find_function(this, function_name);

	if (!address)
	{
		return 0;
	}

	return address - (int)this->address;
}

bool GameLibrary::contains_address(long address)
{
	return (address >= (long)this->address) && (address < ((long)this->address + this->length));
}


const char* GameLibraryEngine::get_name()
{
	return "engine";
}

const char* GameLibraryMod::get_name()
{
	return "mod";
}
