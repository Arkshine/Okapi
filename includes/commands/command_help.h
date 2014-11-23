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

#ifndef __COMMAND_HELP_H__
#define __COMMAND_HELP_H__

#include <command.h>
#include <string.h>

class CommandHelp : public Command
{
	public:

		void exec();
		const char* get_description();
		void show_help();
};

#endif // __COMMAND_HELP_H__
