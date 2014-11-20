
#ifndef __GAME_LIBRARY_H__
#define __GAME_LIBRARY_H__

#include <s_library.h>
#include <heap.h>
#include <s_address_line.h>
#include <CVector.h>
#include <trie.h>
#include <string_.h>
#include <trie_string.h>

class GameLibrary : public s_library
{
	protected:

		void init()
		{
			
		}

	public:

		virtual String get_name() = 0;

		GameLibrary(s_library s)
		{
			this->address = s.address;
			this->length = s.length;
			this->handle = s.handle;
		}

		char* get_address_sym(void* address)
		{
			return get_address_symbol((void*)((int)this->address + (int)address));
		}

		int find_func(char* function_name)
		{
			int address = (int)find_function(this,function_name);

			if(!address)
				return 0;

			return address - (int)this->address;
		}


		bool contains_address(long address)
		{
			return (address >= (long)this->address) && (address < ((long)this->address + this->length));
		}
};

#endif