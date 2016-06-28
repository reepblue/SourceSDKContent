//====== Copyright © 1996-2003, Valve Corporation, All rights reserved. =======
//
// Purpose: Game rules for Scratch
//
//=============================================================================

#ifndef SDK_GAMERULES_H
#define SDK_GAMERULES_H
#ifdef _WIN32
#pragma once
#endif

#include "gamerules.h"
#include "singleplay_gamerules.h"

#ifdef CLIENT_DLL
	#define CSDKGameRules C_SDKGameRules
#endif

class CSDKGameRules : public CSingleplayRules
{
	DECLARE_CLASS( CSDKGameRules, CSingleplayRules );

public:
	bool				IsMultiplayer( void );
	void				PlayerThink( CBasePlayer *pPlayer );
	virtual bool		ShouldCollide( int collisionGroup0, int collisionGroup1 );

#ifdef CLIENT_DLL

	DECLARE_CLIENTCLASS_NOBASE(); // This makes datatables able to access our private vars.

#else

	DECLARE_SERVERCLASS_NOBASE(); // This makes datatables able to access our private vars.

	virtual const char *GetGameDescription( void ) { return "SDK"; }

//	virtual void PlayerThink( CBasePlayer *pPlayer ) {}

//	virtual void PlayerSpawn( CBasePlayer *pPlayer );

	virtual void			InitDefaultAIRelationships( void );
	virtual const char*	AIClassText(int classType);
#endif // CLIENT_DLL

	// misc
//	virtual void CreateStandardEntities( void );	
};

//-----------------------------------------------------------------------------
// Gets us at the SDK game rules
//-----------------------------------------------------------------------------
inline CSDKGameRules* rHGameRules()
{
	return static_cast<CSDKGameRules*>(g_pGameRules);
}

#endif // rh_GAMERULES_H