
#ifndef __COMMAND_RELATION_H__
#define __COMMAND_RELATION_H__

#include <command.h>
#include <my_util.h>
#include <string_.h>
#include <CVector.h>
#include <game_library.h>
#include <globals.h>
#include <work.h>

class CommandRelation : public Command
{
	public:

		void exec()
		{
			int argc = g_engfuncs.pfnCmd_Argc();

			if(argc < 4)
			{
				this->show_help();
				return;
			}

			char* function_identifier1 =  (char*)g_engfuncs.pfnCmd_Argv(2);
			char* function_identifier2 =  (char*)g_engfuncs.pfnCmd_Argv(3);
			
			int address1 = parseHex(function_identifier1);
			int address2 = parseHex(function_identifier2);
			
			CVector<GameLibrary*> libraries;

			libraries.push_back(GameLibraries.Engine);
			libraries.push_back(GameLibraries.Mod);

			bool function1found = false;
			bool function2found = false;

			int foundlevel = 0;
			
			for(size_t i=0;i<libraries.size();i++)
			{
				GameLibrary* library = libraries[i];

				int address1_ = address1;

				if(!address1_)
				{
					address1_ = library->find_func(function_identifier1);
				}

				if(address1_)
				{
					function1found = true;
				}

				int address2_ = address2;

				if(!address2_)
				{
					address2_ = library->find_func(function_identifier2);
				}

				if(address2_)
				{
					function2found = true;
				}

				int f = address1_ || address2_;
				int c = address1_ && address2_;

				if(f != c)
				{
					foundlevel = 1;
				}
				else if(f)
				{
					foundlevel = 2;

					printf("Relation %d\n",library->relationship(address1_,address2_));
				}
			}

			if(!foundlevel)
			{
				printf("None of the functions were found\n");
			}
			else if(foundlevel == 1)
			{
				if(function1found && function2found)
				{
					printf("The functions were found but not on the same library");
				}
				else
				{
					printf("One of the functions wasn't found");
				}
			}
		}

		String get_description()
		{
			String s;

			s.sprintf("gives the relation between to functions. That is, the minimum number of steps in the tree, between them");

			return s;
		}

		void show_help()
		{
			Util::con_printf("usage: okapi relation <function_label1|function_offset1> <function_label2|function_offset2>\n");
		}
};

#endif