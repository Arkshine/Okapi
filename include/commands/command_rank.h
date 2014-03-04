
#ifndef __COMMAND_RANK_H__
#define __COMMAND_RANK_H__

#include <command.h>
#include <my_util.h>
#include <string_.h>
#include <CVector.h>
#include <game_library.h>
#include <globals.h>
#include <work.h>

class CommandRank : public Command
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

			char* treemap =  (char*)g_engfuncs.pfnCmd_Argv(2);
			char* function_identifier =  (char*)g_engfuncs.pfnCmd_Argv(3);

			{
				if(*treemap == '"')
					treemap++;

				int len = strlen(treemap);

				if(( len >= 1) && (*treemap == '"'))
					treemap[len-1] = 0;
			}
		
			String treemap_s(treemap);

			CVector<GameLibrary*> libraries;

			GameLibrary* library = NULL;
			
			libraries.push_back(GameLibraries.Engine);
			libraries.push_back(GameLibraries.Mod);

			for(size_t i=0;i<libraries.size();i++)
			{
				GameLibrary* library_ = libraries[i];

				CVector<int> distances = library_->decode_distances(treemap_s);
				
				if(distances.size())
				{
					library = library_;
					break;
				}
			}

			if(!library)
			{
				printf("invalid treemap given\n");
				return;
			}

			int address = parseHex(function_identifier);

			if(!address)
			{
				address = library->find_func(function_identifier);
			}

			if(!address)
			{
				printf("function not found\n");
				return;
			}

			if(!library->has_address(address))
			{
				printf("function address not in the tree\n");
				return;
			}

			CVector<int> distances = library->decode_distances(treemap_s);

			HeapMax<AddressScore> distances_score_heap = library->calc_distances_score_heap(distances);

			int rank = 0;
					
			int count = 1;

			while(!distances_score_heap.empty())
			{
				AddressScore address_score = distances_score_heap.get();

				int address_ = address_score.address;

				if(address == address_)
				{
					rank = count;
						break;
				}

				count++;
			}

			if(!rank)
			{
				printf("function not ranked\n");
				return;
			}

			printf("function rank %d\n",rank);
				return;
		}

		String get_description()
		{
			String s;

			s.sprintf("given a treemap, and a function, gives the rank of that function in the list of the treemap best matches");

			return s;
		}

		void show_help()
		{
			Util::con_printf("usage: okapi rank <treemap> <function_label|function_offset>\n");
		}
};

#endif