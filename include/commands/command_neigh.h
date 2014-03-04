
#ifndef __COMMAND_NEIGH_H__
#define __COMMAND_NEIGH_H__

#include <command.h>
#include <my_util.h>
#include <string_.h>
#include <CVector.h>
#include <game_library.h>
#include <globals.h>
#include <work.h>

class CommandNeigh : public Command
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

			char* function_identifier =  (char*)g_engfuncs.pfnCmd_Argv(2);

			char* argv_r = (char*)g_engfuncs.pfnCmd_Argv(3);

			int r = atoi(argv_r);
			int r_x = parseHex(argv_r);
				
			if(r_x >= 1 && r_x <= 0xF)
			{
				r = r_x;
			}

			if(r < 1 || r > 0xF)
			{
				printf("Relation must be a number between 1 and 0xF\n");
				return;
			}

			CVector<GameLibrary*> libraries;

			libraries.push_back(GameLibraries.Engine);
			libraries.push_back(GameLibraries.Mod);

			bool found = false;

			for(size_t i=0;i<libraries.size();i++)
			{
				GameLibrary* library = libraries[i];

				int address = get_function_address_from_arg(library,function_identifier);

				if(!address)
					continue;

				found = true;

				String name("");
							
				char* symbol = library->get_address_sym((void*)address);

				if(symbol && strcmp(symbol,""))
				{
					name.sprintf("'%s' ",symbol);
				}

				printf("--------------------\n");
				printf("Function %x %s\n",address,symbol);
				printf("Library %s\n\n",library->get_name().get_ptr());

				CVector<int> neighs = library->get_neighs_at_depth(address,r);

				int c = 0;

				for(size_t i=0;i<neighs.size();i++)
				{
					int neigh = neighs[i];

					int r2 = library->relationship(address,neigh);

					if(r == r2)
					{
						String name_("");
							
						char* symbol_ = library->get_address_sym((void*)neigh);

						if(symbol_ && strcmp(symbol_,""))
						{
							name_.sprintf("'%s' ",symbol_);
						}

						printf("\t%d - 0x%x %s\n",++c,neigh,name_.get_ptr());
					}
				}

				printf("--------------------\n");
				
			}
		}

		String get_description()
		{
			String s;

			s.sprintf("given a function, and a relation number, finds all the neighbors that have the given relation with the function");

			return s;
		}

		void show_help()
		{
			Util::con_printf("usage: okapi neigh <function_label|function_offset> <relation>\n");
		}
};

#endif