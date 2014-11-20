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
