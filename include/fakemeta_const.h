#ifndef __FAKEMETA_CONST_H
#define __FAKEMETA_CONST_H

typedef enum
{
	DLLFunc_GameInit,					// void )			( void );
	DLLFunc_Spawn,						// int  )			(edict_t *pent);
	DLLFunc_Think,						// void )			(edict_t *pent);
	DLLFunc_Use,						// void )			(edict_t *pentUsed, edict_t *pentOther);
	DLLFunc_Touch,						// void )			(edict_t *pentTouched, edict_t *pentOther);
	DLLFunc_Blocked,					// void )			(edict_t *pentBlocked, edict_t *pentOther);

	//You can pass in 0 for glb kvd handle or a kvd handle here
	DLLFunc_KeyValue,					// void )			(edict_t *pentKeyvalue, KeyValueData *pkvd);
	DLLFunc_SetAbsBox,					// void )			(edict_t *pent);
	DLLFunc_ClientConnect,				// bool )			(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128]);

	DLLFunc_ClientDisconnect,			// void )			(edict_t *pEntity);
	DLLFunc_ClientKill,					// void )			(edict_t *pEntity);
	DLLFunc_ClientPutInServer,			// void )			(edict_t *pEntity);
	DLLFunc_ClientCommand,				// void )			(edict_t *pEntity);

	DLLFunc_ServerDeactivate,			// void )			( void );

	DLLFunc_PlayerPreThink,				// void )			(edict_t *pEntity);
	DLLFunc_PlayerPostThink,			// void )			(edict_t *pEntity);

	DLLFunc_StartFrame,					// void )			( void );
	DLLFunc_ParmsNewLevel,				// void )			( void );
	DLLFunc_ParmsChangeLevel,			// void )			( void );

	// Returns string describing current .dll.  E.g., TeamFotrress 2, Half-Life
	// This also gets called when the server is queried for information (for example, by a server browser tool)
	DLLFunc_GetGameDescription,	 		// const char *)	( void );

	// Spectator funcs
	DLLFunc_SpectatorConnect,			// void )			(edict_t *pEntity);
	DLLFunc_SpectatorDisconnect,		// void )			(edict_t *pEntity);
	DLLFunc_SpectatorThink,				// void )			(edict_t *pEntity);

	// Notify game .dll that engine is going to shut down.  Allows mod authors to set a breakpoint.
	DLLFunc_Sys_Error,					// void )			(const char *error_string);

	DLLFunc_PM_FindTextureType,			// char )			(char *name);
	DLLFunc_RegisterEncoders,			// void )			( void );

	// Enumerates player hulls.  Returns 0 if the hull number doesn't exist, 1 otherwise
	DLLFunc_GetHullBounds,				// int  )			(int hullnumber, float *mins, float *maxs);

	// Create baselines for certain "unplaced" items.
	DLLFunc_CreateInstBaselines,		// void )			( void );
	DLLFunc_pfnAllowLagCompensation,	// int  )			( void );
	// I know this does not fit with DLLFUNC(), but I don't want another native just for it.
	MetaFunc_CallGameEntity,			// bool	)			(plid_t plid, const char *entStr,entvars_t *pev);
	DLLFunc_ClientUserInfoChanged,		// void	)			(edict *pEntity, char *infobuffer);
	// You can pass in 0 for global cd handle or another cd handle here
	DLLFunc_UpdateClientData,			// void )			(const struct edict_s *ent, int sendweapons, struct clientdata_s *cd);
	// You can pass in 0 for global entity state handle or another entity state handle here
	DLLFunc_AddToFullPack,				// int  )			(struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet);
	// You can pass in 0 for global usercmd handle or another usercmd handle here
	DLLFunc_CmdStart,					// void )			(const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed);
	DLLFunc_CmdEnd,						// void )			(const edict_t *player);
	DLLFunc_CreateBaseline				// void )			(int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs);
} DLLFunc_Enum;

typedef enum {
	EngFunc_PrecacheModel,				// int  )			(char *s);
	EngFunc_PrecacheSound,				// int  )			(char *s);
	EngFunc_SetModel,					// void )			(edict_t *e, const char *m);
	EngFunc_ModelIndex,					// int  )			(const char *m);
	EngFunc_ModelFrames,				// int	)			(int modelIndex);
	EngFunc_SetSize,					// void )			(edict_t *e, const float *rgflMin, const float *rgflMax);
	EngFunc_ChangeLevel,				// void )			(char* s1, char* s2);
	EngFunc_VecToYaw,					// float)			(const float *rgflVector);
	EngFunc_VecToAngles,				// void )			(const float *rgflVectorIn, float *rgflVectorOut);
	EngFunc_MoveToOrigin,				// void )			(edict_t *ent, const float *pflGoal, float dist, int iMoveType);
	EngFunc_ChangeYaw,					// void )			(edict_t* ent);
	EngFunc_ChangePitch,				// void )			(edict_t* ent);
	EngFunc_FindEntityByString,			// edict)			(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue);
	EngFunc_GetEntityIllum,				// int	)			(edict_t* pEnt);
	EngFunc_FindEntityInSphere,			// edict)			(edict_t *pEdictStartSearchAfter, const float *org, float rad);
	EngFunc_FindClientInPVS,			// edict)			(edict_t *pEdict);
	EngFunc_EntitiesInPVS,				// edict)			(edict_t *pplayer);
	EngFunc_MakeVectors,				// void )			(const float *rgflVector);
	EngFunc_AngleVectors,				// void )			(const float *rgflVector, float *forward, float *right, float *up);
	EngFunc_CreateEntity,				// edict)			(void);
	EngFunc_RemoveEntity,				// void )			(edict_t *e);
	EngFunc_CreateNamedEntity,			// edict)			(int className);
	EngFunc_MakeStatic,					// void )			(edict_t *ent);
	EngFunc_EntIsOnFloor,				// int  )			(edict_t *e);
	EngFunc_DropToFloor,				// int  )			(edict_t *e);
	EngFunc_WalkMove,					// int  )			(edict_t *ent, float yaw, float dist, int iMode);
	EngFunc_SetOrigin,					// void )			(edict_t *e, const float *rgflOrigin);
	EngFunc_EmitSound,					// void )			(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch);
	EngFunc_EmitAmbientSound,			// void )			(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch);

	//With 1.71 you can pass an optional TraceLine ptr for trace natives
	// it can be 0, for meaning "global tr handle" (for get/set_tr2), or
	// it can be any other TR handle (such as one from a TR hook)
	EngFunc_TraceLine,					// void )			(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
	EngFunc_TraceToss,					// void )			(edict_t *pent, edict_t *pentToIgnore, TraceResult *ptr);
	EngFunc_TraceMonsterHull,			// int  )			(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
	EngFunc_TraceHull,					// void )			(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr);
	EngFunc_TraceModel,					// void )			(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr);
	EngFunc_TraceTexture,				// const char *)	(edict_t *pTextureEntity, const float *v1, const float *v2 );
	EngFunc_TraceSphere,				// void )			(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr);

	EngFunc_GetAimVector,				// void )			(edict_t *ent, float speed, float *rgflReturn);
	EngFunc_ParticleEffect,				// void )			(const float *org, const float *dir, float color, float count);
	EngFunc_LightStyle,					// void )			(int style, char *val);
	EngFunc_DecalIndex,					// int  )			(const char *name);
	EngFunc_PointContents,				// int  )			(const float *rgflVector);
	EngFunc_FreeEntPrivateData,			// void )			(edict_t *pEdict);
	EngFunc_SzFromIndex,				// const char *)	(int iString);
	EngFunc_AllocString,				// int  )			(const char *szValue);
	EngFunc_RegUserMsg,					// int	)			(const char *pszName, int iSize);
	EngFunc_AnimationAutomove,			// void )			(const edict_t *pEdict, float flTime);
	EngFunc_GetBonePosition,			// void )			(const edict_t *pEdict, int iBone, float *rgflOrigin, float *rgflAngles);
	EngFunc_GetAttachment,				// void	)			(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles);
	EngFunc_SetView,					// void )			(const edict_t *pClient, const edict_t *pViewent);
	EngFunc_Time,						// float)			( void );
	EngFunc_CrosshairAngle,				// void )			(const edict_t *pClient, float pitch, float yaw);
	EngFunc_FadeClientVolume,			// void )			(const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds);
	EngFunc_SetClientMaxspeed,			// void )			(const edict_t *pEdict, float fNewMaxspeed);
	EngFunc_CreateFakeClient,			// edict)			(const char *netname);	// returns NULL if fake client can't be created
	EngFunc_RunPlayerMove,				// void )			(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec);
	EngFunc_NumberOfEntities,			// int  )			( void );
	EngFunc_StaticDecal,				// void )			(const float *origin, int decalIndex, int entityIndex, int modelIndex);
	EngFunc_PrecacheGeneric,			// int  )			(char* s);
	EngFunc_BuildSoundMsg,				// void )			(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
	EngFunc_GetPhysicsKeyValue,			// const char *)	(const edict_t *pClient, const char *key);
	EngFunc_SetPhysicsKeyValue,			// void )			(const edict_t *pClient, const char *key, const char *value);
	EngFunc_GetPhysicsInfoString,		// const char *)	(const edict_t *pClient);
	EngFunc_PrecacheEvent,				// unsigned short)	(int type, const char*psz);
	EngFunc_PlaybackEvent,				// void )			(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
	EngFunc_CheckVisibility,			// int  )			(const edict_t *entity, unsigned char *pset);
	EngFunc_GetCurrentPlayer,			// int  )			( void );
	EngFunc_CanSkipPlayer,				// int  )			(const edict_t *player);
	EngFunc_SetGroupMask,				// void )			(int mask, int op);
	EngFunc_GetClientListening,			// bool )			(int iReceiver, int iSender)
	EngFunc_SetClientListening,			// bool )			(int iReceiver, int iSender, bool Listen)
	EngFunc_MessageBegin,				// void )			(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
	EngFunc_WriteCoord,					// void )			(float flValue)
	EngFunc_WriteAngle,					// void )			(float flValue)
	EngFunc_InfoKeyValue,				// char*)			(char *infobuffer, char *key);
	EngFunc_SetKeyValue,				// void )			(char *infobuffer, char *key, char *value);
	EngFunc_SetClientKeyValue,			// void )			(int clientIndex, char *infobuffer, char *key, char *value);
	EngFunc_CreateInstBaseline,			// int  )			(int classname, struct entity_state_s *baseline);

	// Returns pointer to info buffer that can be used with the infobuffer param of InfoKeyValue, SetKeyValue, and SetClientKeyValue
	EngFunc_GetInfoKeyBuffer,			// char*)			(edict_t *e);
	EngFunc_AlertMessage,				// void )			(ALERT_TYPE atype, char *szFmt, ...);
	EngFunc_ClientPrintf,				// void )			(edict_t* pEdict, PRINT_TYPE ptype, const char *szMsg);
	EngFunc_ServerPrint					// void )			(const char *szMsg);

} EngFunc_Enum;

#endif