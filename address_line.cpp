
#include <s_address_line.h>

s_address_line::s_address_line(int address,int c_address) : address(address) , c_address(c_address) {}

bool operator==(s_address_line a, s_address_line b)
{
	return a.address == b.address;
}

bool operator<=(s_address_line a, s_address_line b)
{
	return a.address <= b.address;
}

bool operator<(s_address_line a, s_address_line b)
{
	return a.address < b.address;
}

bool operator>(s_address_line a, s_address_line b)
{
	return a.address > b.address;
}