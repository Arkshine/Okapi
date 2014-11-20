#ifndef __S_LIBRARY__H__
#define __S_LIBRARY__H__

struct s_library
{
	void* handle;
	void* address;
	int length;
};

s_library* create_library(void* address);
char* get_address_symbol(void* address);
void* find_function(s_library* library, char* functionName);

#endif
