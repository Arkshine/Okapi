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

#ifndef __FAKEMETA_CONST_TO_ADDRESS_H__
#define __FAKEMETA_CONST_TO_ADDRESS_H__

#include "amxxmodule.h"
#include <fakemeta_const.h>

struct FakemetaConstToAddress
{
	void* engfunc_addresses[EngFunc_Count];
	void* dllfunc_addresses[DLLFunc_Count];

	FakemetaConstToAddress()
	{
		engfunc_addresses[EngFunc_PrecacheModel]           = (void*)g_engfuncs.pfnPrecacheModel;
		engfunc_addresses[EngFunc_PrecacheSound]           = (void*)g_engfuncs.pfnPrecacheSound;
		engfunc_addresses[EngFunc_SetModel]                = (void*)g_engfuncs.pfnSetModel;
		engfunc_addresses[EngFunc_ModelIndex]              = (void*)g_engfuncs.pfnModelIndex;
		engfunc_addresses[EngFunc_ModelFrames]             = (void*)g_engfuncs.pfnModelFrames;
		engfunc_addresses[EngFunc_SetSize]                 = (void*)g_engfuncs.pfnSetSize;
		engfunc_addresses[EngFunc_ChangeLevel]             = (void*)g_engfuncs.pfnChangeLevel;
		engfunc_addresses[EngFunc_VecToYaw]                = (void*)g_engfuncs.pfnVecToYaw;
		engfunc_addresses[EngFunc_VecToAngles]             = (void*)g_engfuncs.pfnVecToAngles;
		engfunc_addresses[EngFunc_MoveToOrigin]            = (void*)g_engfuncs.pfnMoveToOrigin;
		engfunc_addresses[EngFunc_ChangeYaw]               = (void*)g_engfuncs.pfnChangeYaw;
		engfunc_addresses[EngFunc_ChangePitch]             = (void*)g_engfuncs.pfnChangePitch;
		engfunc_addresses[EngFunc_FindEntityByString]      = (void*)g_engfuncs.pfnFindEntityByString;
		engfunc_addresses[EngFunc_GetEntityIllum]          = (void*)g_engfuncs.pfnGetEntityIllum;
		engfunc_addresses[EngFunc_FindEntityInSphere]      = (void*)g_engfuncs.pfnFindEntityInSphere;
		engfunc_addresses[EngFunc_FindClientInPVS]         = (void*)g_engfuncs.pfnFindClientInPVS;
		engfunc_addresses[EngFunc_EntitiesInPVS]           = (void*)g_engfuncs.pfnEntitiesInPVS;
		engfunc_addresses[EngFunc_MakeVectors]             = (void*)g_engfuncs.pfnMakeVectors;
		engfunc_addresses[EngFunc_AngleVectors]            = (void*)g_engfuncs.pfnAngleVectors;
		engfunc_addresses[EngFunc_CreateEntity]            = (void*)g_engfuncs.pfnCreateEntity;
		engfunc_addresses[EngFunc_RemoveEntity]            = (void*)g_engfuncs.pfnRemoveEntity;
		engfunc_addresses[EngFunc_CreateNamedEntity]       = (void*)g_engfuncs.pfnCreateNamedEntity;
		engfunc_addresses[EngFunc_MakeStatic]              = (void*)g_engfuncs.pfnMakeStatic;
		engfunc_addresses[EngFunc_EntIsOnFloor]            = (void*)g_engfuncs.pfnEntIsOnFloor;
		engfunc_addresses[EngFunc_DropToFloor]             = (void*)g_engfuncs.pfnDropToFloor;
		engfunc_addresses[EngFunc_WalkMove]                = (void*)g_engfuncs.pfnWalkMove;
		engfunc_addresses[EngFunc_SetOrigin]               = (void*)g_engfuncs.pfnSetOrigin;
		engfunc_addresses[EngFunc_EmitSound]               = (void*)g_engfuncs.pfnEmitSound;
		engfunc_addresses[EngFunc_EmitAmbientSound]        = (void*)g_engfuncs.pfnEmitAmbientSound;
		engfunc_addresses[EngFunc_TraceLine]               = (void*)g_engfuncs.pfnTraceLine;
		engfunc_addresses[EngFunc_TraceToss]               = (void*)g_engfuncs.pfnTraceToss;
		engfunc_addresses[EngFunc_TraceMonsterHull]        = (void*)g_engfuncs.pfnTraceMonsterHull;
		engfunc_addresses[EngFunc_TraceHull]               = (void*)g_engfuncs.pfnTraceHull;
		engfunc_addresses[EngFunc_TraceModel]              = (void*)g_engfuncs.pfnTraceModel;
		engfunc_addresses[EngFunc_TraceTexture]            = (void*)g_engfuncs.pfnTraceTexture;
		engfunc_addresses[EngFunc_TraceSphere]             = (void*)g_engfuncs.pfnTraceSphere;
		engfunc_addresses[EngFunc_GetAimVector]            = (void*)g_engfuncs.pfnGetAimVector;
		engfunc_addresses[EngFunc_ParticleEffect]          = (void*)g_engfuncs.pfnParticleEffect;
		engfunc_addresses[EngFunc_LightStyle]              = (void*)g_engfuncs.pfnLightStyle;
		engfunc_addresses[EngFunc_DecalIndex]              = (void*)g_engfuncs.pfnDecalIndex;
		engfunc_addresses[EngFunc_PointContents]           = (void*)g_engfuncs.pfnPointContents;
		engfunc_addresses[EngFunc_FreeEntPrivateData]      = (void*)g_engfuncs.pfnFreeEntPrivateData;
		engfunc_addresses[EngFunc_SzFromIndex]             = (void*)g_engfuncs.pfnSzFromIndex;
		engfunc_addresses[EngFunc_AllocString]             = (void*)g_engfuncs.pfnAllocString;
		engfunc_addresses[EngFunc_RegUserMsg]              = (void*)g_engfuncs.pfnRegUserMsg;
		engfunc_addresses[EngFunc_AnimationAutomove]       = (void*)g_engfuncs.pfnAnimationAutomove;
		engfunc_addresses[EngFunc_GetBonePosition]         = (void*)g_engfuncs.pfnGetBonePosition;
		engfunc_addresses[EngFunc_GetAttachment]           = (void*)g_engfuncs.pfnGetAttachment;
		engfunc_addresses[EngFunc_SetView]                 = (void*)g_engfuncs.pfnSetView;
		engfunc_addresses[EngFunc_Time]                    = (void*)g_engfuncs.pfnTime;
		engfunc_addresses[EngFunc_CrosshairAngle]          = (void*)g_engfuncs.pfnCrosshairAngle;
		engfunc_addresses[EngFunc_FadeClientVolume]        = (void*)g_engfuncs.pfnFadeClientVolume;
		engfunc_addresses[EngFunc_SetClientMaxspeed]       = (void*)g_engfuncs.pfnSetClientMaxspeed;
		engfunc_addresses[EngFunc_CreateFakeClient]        = (void*)g_engfuncs.pfnCreateFakeClient;
		engfunc_addresses[EngFunc_RunPlayerMove]           = (void*)g_engfuncs.pfnRunPlayerMove;
		engfunc_addresses[EngFunc_NumberOfEntities]        = (void*)g_engfuncs.pfnNumberOfEntities;
		engfunc_addresses[EngFunc_StaticDecal]             = (void*)g_engfuncs.pfnStaticDecal;
		engfunc_addresses[EngFunc_PrecacheGeneric]         = (void*)g_engfuncs.pfnPrecacheGeneric;
		engfunc_addresses[EngFunc_BuildSoundMsg]           = (void*)g_engfuncs.pfnBuildSoundMsg;
		engfunc_addresses[EngFunc_GetPhysicsKeyValue]      = (void*)g_engfuncs.pfnGetPhysicsKeyValue;
		engfunc_addresses[EngFunc_SetPhysicsKeyValue]      = (void*)g_engfuncs.pfnSetPhysicsKeyValue;
		engfunc_addresses[EngFunc_GetPhysicsInfoString]    = (void*)g_engfuncs.pfnGetPhysicsInfoString;
		engfunc_addresses[EngFunc_PrecacheEvent]           = (void*)g_engfuncs.pfnPrecacheEvent;
		engfunc_addresses[EngFunc_PlaybackEvent]           = (void*)g_engfuncs.pfnPlaybackEvent;
		engfunc_addresses[EngFunc_CheckVisibility]         = (void*)g_engfuncs.pfnCheckVisibility;
		engfunc_addresses[EngFunc_GetCurrentPlayer]        = (void*)g_engfuncs.pfnGetCurrentPlayer;
		engfunc_addresses[EngFunc_CanSkipPlayer]           = (void*)g_engfuncs.pfnCanSkipPlayer;
		engfunc_addresses[EngFunc_SetGroupMask]            = (void*)g_engfuncs.pfnSetGroupMask;
		engfunc_addresses[EngFunc_GetClientListening]      = (void*)g_engfuncs.pfnVoice_GetClientListening;
		engfunc_addresses[EngFunc_SetClientListening]      = (void*)g_engfuncs.pfnVoice_SetClientListening;
		engfunc_addresses[EngFunc_MessageBegin]            = (void*)g_engfuncs.pfnMessageBegin;
		engfunc_addresses[EngFunc_WriteCoord]              = (void*)g_engfuncs.pfnWriteCoord;
		engfunc_addresses[EngFunc_WriteAngle]              = (void*)g_engfuncs.pfnWriteAngle;
		engfunc_addresses[EngFunc_InfoKeyValue]            = (void*)g_engfuncs.pfnInfoKeyValue;
		engfunc_addresses[EngFunc_SetKeyValue]             = (void*)g_engfuncs.pfnSetKeyValue;
		engfunc_addresses[EngFunc_SetClientKeyValue]       = (void*)g_engfuncs.pfnSetClientKeyValue;
		engfunc_addresses[EngFunc_CreateInstBaseline]      = (void*)g_engfuncs.pfnCreateInstancedBaseline;
		engfunc_addresses[EngFunc_GetInfoKeyBuffer]        = (void*)g_engfuncs.pfnGetInfoKeyBuffer;
		engfunc_addresses[EngFunc_AlertMessage]            = (void*)g_engfuncs.pfnAlertMessage;
		engfunc_addresses[EngFunc_ClientPrintf]            = (void*)g_engfuncs.pfnClientPrintf;
		engfunc_addresses[EngFunc_ServerPrint]             = (void*)g_engfuncs.pfnServerPrint;

		dllfunc_addresses[DLLFunc_GameInit]                = (void*)gpGamedllFuncs->dllapi_table->pfnGameInit;
		dllfunc_addresses[DLLFunc_Spawn]                   = (void*)gpGamedllFuncs->dllapi_table->pfnSpawn;
		dllfunc_addresses[DLLFunc_Think]                   = (void*)gpGamedllFuncs->dllapi_table->pfnThink;
		dllfunc_addresses[DLLFunc_Use]                     = (void*)gpGamedllFuncs->dllapi_table->pfnUse;
		dllfunc_addresses[DLLFunc_Touch]                   = (void*)gpGamedllFuncs->dllapi_table->pfnTouch;
		dllfunc_addresses[DLLFunc_Blocked]                 = (void*)gpGamedllFuncs->dllapi_table->pfnBlocked;
		dllfunc_addresses[DLLFunc_KeyValue]                = (void*)gpGamedllFuncs->dllapi_table->pfnKeyValue;
		dllfunc_addresses[DLLFunc_SetAbsBox]               = (void*)gpGamedllFuncs->dllapi_table->pfnSetAbsBox;
		dllfunc_addresses[DLLFunc_ClientConnect]           = (void*)gpGamedllFuncs->dllapi_table->pfnClientConnect;
		dllfunc_addresses[DLLFunc_ClientDisconnect]        = (void*)gpGamedllFuncs->dllapi_table->pfnClientDisconnect;
		dllfunc_addresses[DLLFunc_ClientKill]              = (void*)gpGamedllFuncs->dllapi_table->pfnClientKill;
		dllfunc_addresses[DLLFunc_ClientPutInServer]       = (void*)gpGamedllFuncs->dllapi_table->pfnClientPutInServer;
		dllfunc_addresses[DLLFunc_ClientCommand]           = (void*)gpGamedllFuncs->dllapi_table->pfnClientCommand;
		dllfunc_addresses[DLLFunc_ServerDeactivate]        = (void*)gpGamedllFuncs->dllapi_table->pfnServerDeactivate;
		dllfunc_addresses[DLLFunc_PlayerPreThink]          = (void*)gpGamedllFuncs->dllapi_table->pfnPlayerPreThink;
		dllfunc_addresses[DLLFunc_PlayerPostThink]         = (void*)gpGamedllFuncs->dllapi_table->pfnPlayerPostThink;
		dllfunc_addresses[DLLFunc_StartFrame]              = (void*)gpGamedllFuncs->dllapi_table->pfnStartFrame;
		dllfunc_addresses[DLLFunc_ParmsNewLevel]           = (void*)gpGamedllFuncs->dllapi_table->pfnParmsNewLevel;
		dllfunc_addresses[DLLFunc_ParmsChangeLevel]        = (void*)gpGamedllFuncs->dllapi_table->pfnParmsChangeLevel;
		dllfunc_addresses[DLLFunc_GetGameDescription]      = (void*)gpGamedllFuncs->dllapi_table->pfnGetGameDescription;
		dllfunc_addresses[DLLFunc_SpectatorConnect]        = (void*)gpGamedllFuncs->dllapi_table->pfnSpectatorConnect;
		dllfunc_addresses[DLLFunc_SpectatorDisconnect]     = (void*)gpGamedllFuncs->dllapi_table->pfnSpectatorDisconnect;
		dllfunc_addresses[DLLFunc_SpectatorThink]          = (void*)gpGamedllFuncs->dllapi_table->pfnSpectatorThink;
		dllfunc_addresses[DLLFunc_Sys_Error]               = (void*)gpGamedllFuncs->dllapi_table->pfnSys_Error;
		dllfunc_addresses[DLLFunc_PM_FindTextureType]      = (void*)gpGamedllFuncs->dllapi_table->pfnPM_FindTextureType;
		dllfunc_addresses[DLLFunc_RegisterEncoders]        = (void*)gpGamedllFuncs->dllapi_table->pfnRegisterEncoders;
		dllfunc_addresses[DLLFunc_GetHullBounds]           = (void*)gpGamedllFuncs->dllapi_table->pfnGetHullBounds;
		dllfunc_addresses[DLLFunc_CreateInstBaselines]     = (void*)gpGamedllFuncs->dllapi_table->pfnCreateInstancedBaselines;
		dllfunc_addresses[DLLFunc_pfnAllowLagCompensation] = (void*)gpGamedllFuncs->dllapi_table->pfnAllowLagCompensation;
		dllfunc_addresses[MetaFunc_CallGameEntity]         = (void*)gpMetaUtilFuncs->pfnCallGameEntity;
		dllfunc_addresses[DLLFunc_ClientUserInfoChanged]   = (void*)gpGamedllFuncs->dllapi_table->pfnClientUserInfoChanged;
		dllfunc_addresses[DLLFunc_UpdateClientData]        = (void*)gpGamedllFuncs->dllapi_table->pfnUpdateClientData;
		dllfunc_addresses[DLLFunc_AddToFullPack]           = (void*)gpGamedllFuncs->dllapi_table->pfnAddToFullPack;
		dllfunc_addresses[DLLFunc_CmdStart]                = (void*)gpGamedllFuncs->dllapi_table->pfnCmdStart;
		dllfunc_addresses[DLLFunc_CmdEnd]                  = (void*)gpGamedllFuncs->dllapi_table->pfnCmdEnd;
		dllfunc_addresses[DLLFunc_CreateBaseline]          = (void*)gpGamedllFuncs->dllapi_table->pfnCreateBaseline;
	}
};

#endif // __FAKEMETA_CONST_TO_ADDRESS_H__

