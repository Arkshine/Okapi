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

#include <command.h>

int parseHex(char *s)
{
	if(!*s)
		return 0;

	if(*s++ != '0')
		return 0;
			
	if(*s++ != 'x')
		return 0;

	int mult = 1;
	int sum = 0;

	char c;

	while(c=*s++)
	{
		if((c >= '0') && (c <='9'))
		{
			c -= '0';
		}
		else if((c >= 'a') && (c <= 'f'))
		{
			c += 10 - 'a';
		}
		else if((c >= 'A') && (c <= 'F'))
		{
			c += 10 - 'A';
		}
		else
		{
			return 0;
		}

		sum *= 0x10;
		sum += c;
	}

	return sum;
}
