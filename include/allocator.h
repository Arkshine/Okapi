#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <CVector.h>

class Allocator
{
	CVector<void*> data;

public:

	~Allocator()
	{
		this->clear();
	}

	void clear()
	{
		for (size_t i=0; i < data.size(); i++)
		{
			delete data[i];
		}

		data.clear();
	}

	typedef CVector<void*>::iterator iterator;

	bool free_ptr(void* ptr)
	{
		for (iterator it = data.begin(); it < data.end(); it++)
		{
			if (*it == ptr)
			{
				delete ptr;

				data.erase(it);

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
		data.push_back((void*)block);

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

#endif