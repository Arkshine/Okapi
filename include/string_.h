#ifndef __STRING_H__
#define __STRING_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "amxxmodule.h"

class String
{
	char* data;
	size_t size;
	int pos;

	void init(size_t size)
	{
		this->size = size;
		data = new char[this->size + 1];

		data[0] = 0;
		pos = 0;
	}

public:

	String(size_t size=1)
	{
		init(size);
	}

	String(const String& other)
	{
		this->size = other.size;
		this->pos = other.pos;

		this->data = new char[this->size + 1];
		memcpy(this->data, other.data, pos);

		this->data[pos] = 0;
	}

	String(char *s)
	{
		init(1);

		this->sprintf("%s", s);
	}

	String copy()
	{
		String a;

		a.sprintf("%s", this->data);

		return a;
	}

	String& operator=(String other)
	{
		if (this == &other)
			return *this;

		char* aux_data = other.data;
		int aux_size = other.size;
		int aux_pos = other.pos;

		other.data = data;
		other.pos = pos;
		other.size = size;

		this->data = aux_data;
		this->size = aux_size;
		this->pos = aux_pos;

		return *this;
	}

	String& operator+=(char c)
	{
		char holder[2];
		holder[0] = c;
		holder[1] = 0;

		this->sprintf("%c", c);

		return *this;
	}

	String& operator+=(char* c)
	{
		this->sprintf("%s", c);

		return *this;
	}

	String &operator--()
	{
		if (this->pos)
		{
			this->pos--;
			data[this->pos] = 0;
		}

		return *this;
	}

	void sprintf(char *format, ...)
	{
		while (true)
		{
			va_list args;
			va_start(args, format);

			int len = size - pos;

#if defined __linux__
			int zero = 1;
#else
			int zero = 0;
#endif
			int c = vsnprintf(&data[pos], len + zero, format, args);

			va_end(args);

			if ((c < 0) || (c > len))
			{
				this->size *= 2;
				data = (char*)realloc(data, this->size + 1);
				continue;
			}
			else
			{
				pos += c;
				data[pos] = 0;
			}

			return;
		}
	}

	void show()
	{
		printf(this->data);
		printf("\n");
	}

	~String()
	{
		delete[] data;
	}

	char* get_ptr()
	{
		return this->data;
	}
};

#endif