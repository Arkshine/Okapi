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

#ifndef __S_ADDRESS_LINE__H__
#define __S_ADDRESS_LINE__H__

struct s_address_line
{
	public:

		int address;
		int c_address;

	public:

		s_address_line(int address=0, int c_address=0);
};

bool operator==(s_address_line a, s_address_line b);
bool operator<=(s_address_line a, s_address_line b);
bool operator<(s_address_line a, s_address_line b);
bool operator>(s_address_line a, s_address_line b);

#endif // __S_ADDRESS_LINE__H__

