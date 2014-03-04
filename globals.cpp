
#include <globals.h>

#include <game_library.h>
#include <trie_string.h>
#include <command.h>
#include <offset_handler.h>
#include <function.h>
#include <my_util.h>

GameLibrariesType GameLibraries = {NULL,NULL};
TrieString<Command*> Commands;
Memory G_Memory;
OffsetHandler* G_OffsetHandler = NULL;
HL_TypeConversion G_HL_TypeConversion;
FakemetaConstToAddress* G_FakemetaConstToAddress;

Trie10<Function*> G_Functions;

Allocator G_Allocator;

TypeHandler* G_ArgsTypeHandler[9] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

using Util::con_printf;

void list_commands()
{
	con_printf("available commands:\n\n");

	CVector<String> command_names = Commands.get_keys();

	for(size_t i=0;i<command_names.size();i++)
	{
		String command_name = command_names[i];

		if(command_name.get_ptr() == "help")
			continue;

		Command* command = Commands[command_name.get_ptr()];

		con_printf("\t%s - %s\n",command_names[i].get_ptr(),command->get_description().get_ptr());
	}
}
