//=========== Copyright © 2012, rHetorical, All rights reserved. =============
//
// Purpose: A launchpad based off the launch pad from Blue Portals. 
// But unlike Blue Portals, in Punt players can punt surfaces freely.
//		
//=============================================================================

#include "cbase.h"
#include "player.h"
#include "filters.h"
#include "func_break.h"
#include "decals.h"
#include "explode.h"
#include "in_buttons.h"
#include "physics.h"
#include "IEffects.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "SoundEmitterSystem/isoundemittersystembase.h"
#include "globals.h"
#include "util.h"
#include "physics_impact_damage.h"
#include "tier0/icommandline.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CFuncPuntPad : public CBaseEntity
{
public:
	DECLARE_CLASS( CFuncPuntPad, CBaseEntity );
	DECLARE_DATADESC();
 
	void Spawn();
	bool CreateVPhysics( void );
	void OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason ); 
	
private:

	void InputEnable( inputdata_t &inputdata );
	void InputDisable( inputdata_t &inputdata );
	bool m_bDisabled;

	CHandle<CBasePlayer>	m_hPhysicsAttacker;
};

LINK_ENTITY_TO_CLASS( func_punt_surface, CFuncPuntPad );
 
BEGIN_DATADESC( CFuncPuntPad )

	DEFINE_KEYFIELD(m_bDisabled, FIELD_BOOLEAN, "StartDisabled"),
	
	DEFINE_INPUTFUNC(FIELD_VOID, "Enable", InputEnable),
	DEFINE_INPUTFUNC(FIELD_VOID, "Disable", InputDisable),

	// Declare this function as being the touch function
	//DEFINE_ENTITYFUNC( BrushTouch ),
 
END_DATADESC()

bool CFuncPuntPad::CreateVPhysics( void )
{
	VPhysicsInitStatic();
	return true;
}

void CFuncPuntPad::Spawn()
{
	BaseClass::Spawn();

	// We want to capture touches from other entities
	//SetTouch( &CFuncPuntPad::BrushTouch );
 
	// We should collide with physics
	SetSolid( SOLID_BSP );
 
	// Use our brushmodel
	SetModel( STRING( GetModelName() ) );
 
	// We push things out of our way
	//SetMoveType( MOVETYPE_NONE );
	SetMoveType( MOVETYPE_PUSH );
	AddFlag( FL_WORLDBRUSH );
	m_takedamage = DAMAGE_YES;

	SetCollisionGroup( COLLISION_GROUP_PUNTABLE );

	CreateVPhysics();
}

void CFuncPuntPad::InputEnable( inputdata_t &inputdata )
{
	m_bDisabled = false;
	SetCollisionGroup( COLLISION_GROUP_PUNTABLE );
	m_takedamage = DAMAGE_YES;
}

void CFuncPuntPad::InputDisable( inputdata_t &inputdata )
{
	m_bDisabled = true;
	SetCollisionGroup( COLLISION_GROUP_UNPUNTABLE );
	m_takedamage = DAMAGE_NO;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFuncPuntPad::OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason )
{
	m_hPhysicsAttacker = pPhysGunUser;

	if( reason == PUNTED_BY_CANNON )
	{
			//pPhysGunUser->ApplyLocalVelocityImpulse( launchforce );
			DevMsg ( "func_punt_launchpad was punted!");
	}
}