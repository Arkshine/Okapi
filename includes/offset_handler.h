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

#ifndef OFFSET_HANDLER_H
#define OFFSET_HANDLER_H

struct OffsetHandler
{
	size_t PEV;
	size_t EntityVirtualTable;

	void search_pev();
	void search_virtual_table();

	OffsetHandler()
	{
		search_pev();
		search_virtual_table();
	}

	static size_t search_virtual_table(void *address);
};

#endif // OFFSET_HANDLER_H

