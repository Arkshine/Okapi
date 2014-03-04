
#ifndef __COMMAND_DESCRIBE_H__
#define __COMMAND_DESCRIBE_H__

#include <command.h>
#include <my_util.h>

class CommandDescribe : public Command
{
		void show_function(GameLibrary* library,int address)
		{
			printf("--------------------\n");

			printf("Function: 0x%x\n",address);

			char* symbol = library->get_address_sym((void*)address);

			if(symbol)
			{
				printf("Symbol: %s\n",symbol);
			}

			printf("Library: %s\n",library->get_name().get_ptr());

			CVector<int> distances = library->get_distances(address);
			String treemap = library->encode_distances(distances);

			printf("Treemap: \"%s\"\n",treemap.get_ptr());

			printf("\nMap values {");

			printf("%x",distances[0]);

			for(size_t i=1;i<distances.size();i++)
			{
				printf(",%x",distances[i]);
			}

			printf("}\n");

			printf("--------------------\n");
		}

		void describe(CVector<GameLibrary*> libraries,int argc,int next_arg)
		{
			if(next_arg+1 != argc)
			{
				Util::con_printf("missing function identifier\n");
				return;
			}

			char* argv = (char*)g_engfuncs.pfnCmd_Argv(next_arg);

			int address = parseHex(argv);

			bool found = false;

			for(size_t i=0;i<libraries.size();i++)
			{
				GameLibrary* library = libraries[i];

				if(address)
				{
					bool has = library->has_address(address);

					if(has)
					{
						show_function(library,address);
						found = true;
					}
				}
				else
				{
					int address_ = library->find_func(argv);

					if(address_)
					{
						bool has = library->has_address(address_);

						if(has)
						{
							show_function(library,address_);
							found = true;
						}
					}
				}
			}	
			
			if(!found)
				printf("Not found '%s'\n",argv);
		}

	public:

		void exec()
		{
			int argc = g_engfuncs.pfnCmd_Argc();

			if(argc < 3)
			{
				this->show_help();
				return;
			}

			char* argv = (char*)g_engfuncs.pfnCmd_Argv(2);
		
			CVector<GameLibrary*> libraries;

			if(!strcmp(argv,"engine"))
			{
				libraries.push_back(GameLibraries.Engine);
			}
			else if(!strcmp(argv,"mod"))
			{
				libraries.push_back(GameLibraries.Mod);
			}
			else
			{
				libraries.push_back(GameLibraries.Engine);
				libraries.push_back(GameLibraries.Mod);
			}

			describe(libraries,argc, 2 + (libraries.size() == 1));
		}

		String get_description()
		{
			String s;

			s.sprintf("describes a function, giving its treemap");

			return s;
		}

		void show_help()
		{
			Util::con_printf("usage: okapi desc [engine|mod] <function_label|function_offset>\n");
		}
};

#endif