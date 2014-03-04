
#ifndef __COMMAND_FIND_H__
#define __COMMAND_FIND_H__

#include <command.h>
#include <my_util.h>
#include <string_.h>
#include <func_relation.h>

class CommandSearch : public Command
{

		CVector<FuncRelation> parse_relations(GameLibrary* library)
		{
			CVector<FuncRelation> relations;

			int argc = g_engfuncs.pfnCmd_Argc();
			int curr_argv_n = 3;

			while(curr_argv_n < argc)
			{
				char* argv_1 = (char*)g_engfuncs.pfnCmd_Argv(curr_argv_n);
				int r = atoi(g_engfuncs.pfnCmd_Argv(curr_argv_n+1));
				int r_x = parseHex((char*)g_engfuncs.pfnCmd_Argv(curr_argv_n+1));

				int address = get_function_address_from_arg(library,argv_1);
				
				if(!address)
				{
					printf("Function '%s' not found\n",argv_1);
					return CVector<FuncRelation>();
				}
				
				if(r_x >= 1 && r_x <= 0xF)
				{
					r = r_x;
				}

				if(r < 1 || r > 0xF)
				{
					printf("Relation must be a number between 1 and 0xF\n",argv_1);
					return CVector<FuncRelation>();
				}

				relations.push_back(FuncRelation(address,r));

				curr_argv_n+=2;
			}

			return relations;
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

			if((argc%2) != 1)
			{
				printf("wrong number of arguments");
				return;
			}

			char* argv = (char*)g_engfuncs.pfnCmd_Argv(2);
		
			if(*argv == '"')
				argv++;

			int len = strlen(argv);

			if(( len >= 1) && (*argv == '"'))
				argv[len-1] = 0;

			String s(argv);

			CVector<GameLibrary*> libraries;

			bool found = false;

			libraries.push_back(GameLibraries.Engine);
			libraries.push_back(GameLibraries.Mod);

			int search_min = 1;
			int search_max = 30;

			for(size_t i=0;i<libraries.size();i++)
			{
				GameLibrary* library = libraries[i];

				CVector<int> distances = library->decode_distances(s);

				if(distances.size())
				{
					found = true;

					CVector<FuncRelation> relations = parse_relations(library);
					Trie10< Trie10<bool> > restritions = library->parse_restritions(relations);

					if((argc > 3) && (!restritions.get_keys().size()))
					{
						break;
					}

					HeapMax<AddressScore> distances_score_heap = library->calc_distances_score_heap(distances);

					printf("Treemap of %s.\n\n",library->get_name().get_ptr());
					
					int c = 1;
					
					while(!distances_score_heap.empty())
					{
						AddressScore address_score = distances_score_heap.get();

						int address = address_score.address;
						float score = address_score.score;

						CVector<int> addresses = restritions.get_keys();

						bool satisfied = true;

						for(size_t i=0;i<addresses.size();i++)
						{
							int relation_address = addresses[i];

							if(!restritions[relation_address][address])
							{
								satisfied = false;
								break;
							}
						}

						if(!satisfied)
							continue;

						if(c >= search_min && c <= search_max)
						{
							String name("");
							
							char* symbol = library->get_address_sym((void*)address);

							if(symbol && strcmp(symbol,""))
							{
								name.sprintf("'%s' ",symbol);
							}

							printf("\tFunction: 0x%x %s- Score: %f\n",address,name.get_ptr(),score);
						}

						if(c > search_max)
							break;

						c++;
					}

					break;
				}
			}

			if(!found)
			{
				printf("invalid treemap given\n");
			}
		}

		String get_description()
		{
			String s;

			s.sprintf("given a treemap, finds its best possible matches ordered by score. you can also provide a list of additional restritions");

			return s;
		}

		void show_help()
		{
			Util::con_printf("usage: okapi search <treemap> [<function_label1|function_offset1> <relation 1> ... <function_label_n|function_offset_n> <relation n>]\n");
		}
};

#endif