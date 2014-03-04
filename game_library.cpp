
#include <game_library.h>
#include <globals.h>

void GameLibrary::fill_call_tree(HeapMin<s_address_line> call_list)
{
	int address = 0;
		
	while(!call_list.empty())
	{
		s_address_line x = call_list.get();

		if(!x.c_address)
			address = x.address;
		else
		{	 
			this->call_tree[address][address] = true;
			this->call_tree[x.c_address][x.c_address] = true;

			this->call_tree[address][x.c_address] = true;
			this->call_tree[x.c_address][address] = true;
		}
	}
}
