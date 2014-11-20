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
#include <commands/command_help.h>
#include <my_util.h>
#include "amxxmodule.h"
#include <globals.h>

void CommandHelp::exec()
{
	if (g_engfuncs.pfnCmd_Argc() == 2)
	{
		Util::con_printf("Usage: okapi help <command>\n");
		Util::con_printf("\n");

		list_commands();
	}
	else
	{
		Command* command = Commands[(char*)g_engfuncs.pfnCmd_Argv(2)];

		if (!command)
			printf("invalid command\n");
		else
		{
			command->show_help();
		}
	}
}

String CommandHelp::get_description()
{
	String s;

	s.sprintf("shows the help text of a command");

	return s;
}

void CommandHelp::show_help()
{
	Util::con_printf("usage: okapi help <command>\n");
}
