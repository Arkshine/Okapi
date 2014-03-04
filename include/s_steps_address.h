
#ifndef __S_ADDRESS_STEPS__H__
#define __S_ADDRESS_STEPS__H__

struct s_steps_address
{
	int address;
	int steps;

	s_steps_address(int address=0,int steps=0);
};

bool operator <(s_steps_address a,s_steps_address b);


#endif
