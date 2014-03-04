
#include <s_steps_address.h>

s_steps_address::s_steps_address(int address,int steps)  : address(address), steps(steps){ }

bool operator <(s_steps_address a,s_steps_address b)
{
	return a.steps < b.steps;
}

