
#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string_.h>
#include <game_library.h>

class Command
{
	public:

		virtual void show_help() = 0;
		virtual void exec() = 0;
		virtual String get_description() = 0;
};

int parseHex(char *s);

#endif
