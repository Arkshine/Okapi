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

#include "globals.h"
#include "natives.h"
#include "utils.h"
#include "s_library.h"
#include "game_library.h"
#include "function.h"
#include "type_handler.h"

using Util::con_printf;

void OnAmxxAttach()
{
	MF_AddNatives(OkapiNatives);
}

void OnOkapiCommand()
{
	con_printf("\n######### OKAPI #########\n\n");

	if (CMD_ARGC() == 1)
	{
		con_printf("okapi <command>\n");
		con_printf("okapi help <command>\n");
		con_printf("\n");

		list_commands();
	}
	else
	{
		Command* command;

		if (!G_Commands.retrieve(CMD_ARGV(1), &command))
		{
			con_printf("invalid command\n");
		}
		else
		{
			command->exec();
		}
	}

	con_printf("\n#########################\n\n");
}

void OnMetaAttach(void)
{
	s_library* mod_library = create_library((void*)gpGamedllFuncs->dllapi_table->pfnSpawn);

	if (!mod_library)
	{
		mod_library = create_library((void*)MDLL_Spawn);
	}

	s_library* engine_library = create_library((void*)gpGlobals);

	G_GameLibraries.Engine = new GameLibraryEngine(*engine_library);
	G_GameLibraries.Mod    = new GameLibraryMod(*mod_library);

	REG_SVR_COMMAND("okapi", OnOkapiCommand);

	G_Commands.insert("help"   , new CommandHelp());
	G_Commands.insert("version", new CommandVersion());

	if (!G_FakemetaConstToAddress)
	{
		G_FakemetaConstToAddress = new FakemetaConstToAddress();
	}

	G_ArgsTypeHandler[ArgVoid]	  = new VoidHandler();
	G_ArgsTypeHandler[ArgInt]	  = new IntHandler();
	G_ArgsTypeHandler[ArgCBase]   = new CBaseHandler();
	G_ArgsTypeHandler[ArgFloat]	  = new FloatHandler();
	G_ArgsTypeHandler[ArgVec]	  = new VecHandler();
	G_ArgsTypeHandler[ArgEntvars] = new EntvarsHandler();
	G_ArgsTypeHandler[ArgStr]	  = new StrHandler();
	G_ArgsTypeHandler[ArgEdict]	  = new EdictHandler();
	G_ArgsTypeHandler[ArgVecPtr]  = new VecPtrHandler();
}

void OnPluginsLoaded()
{
	if (!G_OffsetHandler)
	{
		G_OffsetHandler = new OffsetHandler();
	}

	G_Functions.init();
}

void OnPluginsUnloaded()
{
	Function::clean();

	G_Functions.clear();
	G_Allocator.clear();
}

extern "C" void __cxa_pure_virtual() { while (1); }