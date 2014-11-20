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

#ifndef __OFFSET_HANDLER_H__
#define __OFFSET_HANDLER_H__

struct OffsetHandler
{
	int PEV;
	int EntityVirtualTable;

	void search_pev();
	void search_virtual_table();

	OffsetHandler()
	{
		search_pev();
		search_virtual_table();
	}

	static int search_virtual_table(void *address);
};

#endif // __OFFSET_HANDLER_H__

