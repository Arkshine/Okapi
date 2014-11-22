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

#ifndef __HL_TYPE_CONVERSION_H__
#define __HL_TYPE_CONVERSION_H__

#include "amxxmodule.h"

class HL_TypeConversion
{
	public:
		// From fakemeta
		inline edict_t* INDEXENT2(int iEdictNum)
		{
			if (iEdictNum >= 1 && iEdictNum <= gpGlobals->maxClients)
				return MF_GetPlayerEdict(iEdictNum);
			else
				return (*g_engfuncs.pfnPEntityOfEntIndex)(iEdictNum);
		}

		edict_t* entvar_to_edict(entvars_t *pev)
		{
			if (pev == NULL)
			{
				return NULL;
			}

			return pev->pContainingEntity;
		}

		int entvar_to_id(entvars_t *pev)
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

		void* id_to_cbase(int index)
		{
			return INDEXENT2(index)->pvPrivateData;
		}

		entvars_t* id_to_entvar(int index)
		{
			return &(INDEXENT2(index)->v);
		}

		entvars_t* cbase_to_edict(void* cbase);

		int cbase_to_id(void *cbase)
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
};

#endif // __HL_TYPE_CONVERSION_H__
