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

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <game_library.h>
#include <sm_stringhashmap.h>
#include <command.h>
#include <memory_.h>
#include <offset_handler.h>
#include <hl_type_conversion.h>
#include <fakemeta_const_to_address.h>
#include <trie.h>
#include <function.h>
#include <allocator.h>

struct GameLibrariesType
{
	GameLibrary* Mod;
	GameLibrary* Engine;
};

extern GameLibrariesType GameLibraries;
extern StringHashMap<Command*> Commands;

extern Memory G_Memory;

extern OffsetHandler* G_OffsetHandler;

extern HL_TypeConversion G_HL_TypeConversion;

extern FakemetaConstToAddress* G_FakemetaConstToAddress;

extern Trie10<Function*> G_Functions;

extern Allocator G_Allocator;

enum ArgsType
{
	ArgVoid,
	ArgInt,
	ArgCBase,
	ArgFloat,
	ArgVec,
	ArgEntvars,
	ArgStr,
	ArgEdict,
	ArgVecPtr
};

extern TypeHandler* G_ArgsTypeHandler[9];

void list_commands();

#endif // __GLOBALS_H__
