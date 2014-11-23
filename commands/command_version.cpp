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

#include <command.h>
#include <utils.h>
#include "amxxmodule.h"
#include <globals.h>

void CommandVersion::exec()
{
	Util::con_printf(" %s %s\n -\n", Plugin_info.name, OKAPI_VERSION);
	Util::con_printf(" Support      : %s\n", Plugin_info.url);
	Util::con_printf(" Author       : %s\n", Plugin_info.author);
	Util::con_printf(" Compiled on: : %s\n", OKAPI_BUILD_TIME);
#if defined(OKAPI_GENERATED_BUILD)
	Util::con_printf(" Built from   : https://github.com/Arkshine/Okapi/commit/%s\n", OKAPI_SHA);
	Util::con_printf(" Build ID     : %s:%s\n", OKAPI_LOCAL_REV, OKAPI_SHA);
#endif
}

const char* CommandVersion::get_description()
{
	return "shows module informations\n";
}

void CommandVersion::show_help()
{
	Util::con_printf(" usage: okapi version\n");
}