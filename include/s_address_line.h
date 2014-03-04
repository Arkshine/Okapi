
#ifndef __S_ADDRESS_LINE__H__
#define __S_ADDRESS_LINE__H__



struct s_address_line
{
	int address;
	int c_address;

	public:
	
		s_address_line(int address=0,int c_address=0);
};

bool operator==(s_address_line a, s_address_line b);
bool operator<=(s_address_line a, s_address_line b);
bool operator<(s_address_line a, s_address_line b);
bool operator>(s_address_line a, s_address_line b);


#endif
