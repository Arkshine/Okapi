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

#endif
