//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

//
// Okapi Module
//

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <am-vector.h>

class Allocator
{
	ke::Vector<uintptr_t*> data;

	public:

		~Allocator()
		{
			this->clear();
		}

		void clear()
		{
			for (size_t i = 0; i < data.length(); ++i)
			{
				delete data[i];
			}

			data.clear();
		}

		bool free_ptr(uintptr_t* ptr)
		{
			for (size_t i = 0; i < data.length(); ++i)
			{
				if (data[i] == ptr)
				{
					delete ptr;

					data.remove(i);

					return true;
				}
			}

			return false;
		}

		Allocator()
		{
		}

		template <class T>
		T* alloc(int size)
		{
			T* block = new T[size];
			data.append((uintptr_t*)block);

			return block;
		}

		Allocator(const Allocator& obj)
		{
			assert(0);
		}

		Allocator& operator= (Allocator other)
		{
			assert(0);

			return *this;
		}
};

#endif // ALLOCATOR_H
