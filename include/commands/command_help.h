
#ifndef __COMMAND_HELP_H__
#define __COMMAND_HELP_H__

#include <command.h>
#include <string.h>

class CommandHelp : public Command
{
	public:

		void exec();
		String get_description();
		void show_help();
};

#endif