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

#include "hl_type_conversion.h"
#include "globals.h"

edict_t* HL_TypeConversion::INDEXENT2(int iEdictNum)
{
	if (iEdictNum >= 1 && iEdictNum <= gpGlobals->maxClients)
	{
		return MF_GetPlayerEdict(iEdictNum);
	}
	else
	{
		return (*g_engfuncs.pfnPEntityOfEntIndex)(iEdictNum);
	}
}

edict_t* HL_TypeConversion::entvar_to_edict(entvars_t *pev)
{
	if (pev == NULL)
	{
		return NULL;
	}

	return pev->pContainingEntity;
}

int HL_TypeConversion::entvar_to_id(entvars_t *pev)
{
	if (pev == NULL)
	{
		return -1;
	}

	if (pev->pContainingEntity == NULL)
	{
		return -1;
	}

	return ENTINDEX(pev->pContainingEntity);
}

void* HL_TypeConversion::id_to_cbase(int index)
{
	return INDEXENT2(index)->pvPrivateData;
}

entvars_t* HL_TypeConversion::id_to_entvar(int index)
{
	return &(INDEXENT2(index)->v);
}

entvars_t* HL_TypeConversion::cbase_to_edict(void* cbase)
{
	if (!cbase)
	{
		return NULL;
	}

	return *(entvars_t **)((char *)(cbase) /*+ G_OffsetHandler->PEV*/);
}

int HL_TypeConversion::cbase_to_id(void *cbase)
{
	if (cbase == NULL)
	{
		return -1;
	}

	entvars_t* pev = this->cbase_to_edict(cbase);

	if (pev == NULL)
	{
		return -1;
	}

	if (pev->pContainingEntity == NULL)
	{
		return -1;
	}

	return ENTINDEX(pev->pContainingEntity);
}
