//=========== Copyright © 2012, rHetorical, All rights reserved. =============
//
// Purpose: 
//		
//=============================================================================

#include "cbase.h"
#include "doors.h"
#include "mathlib/mathlib.h"
#include "physics.h"
#include "ndebugoverlay.h"
#include "engine/IEngineSound.h"
#include "globals.h"
#include "filters.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CFuncNoPunt : public CBaseEntity
{
public:
	DECLARE_CLASS( CFuncNoPunt, CBaseEntity );
	DECLARE_DATADESC();
 
	void Spawn();
	bool CreateVPhysics( void );

	void InputEnable( inputdata_t &data );
	void InputDisable( inputdata_t &data );

	bool							m_bDisabled;

};
 
LINK_ENTITY_TO_CLASS( func_nopunt, CFuncNoPunt );
 
BEGIN_DATADESC( CFuncNoPunt )
 
	DEFINE_FIELD( m_bDisabled,	FIELD_BOOLEAN ),

	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),
 
END_DATADESC()

void CFuncNoPunt::Spawn()
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

	SetCollisionGroup( COLLISION_GROUP_UNPUNTABLE );

	CreateVPhysics();
}

bool CFuncNoPunt::CreateVPhysics( void )
{
	VPhysicsInitStatic();
	return true;
}

void CFuncNoPunt::InputEnable( inputdata_t &data )
{
	m_bDisabled = false;
}

void CFuncNoPunt::InputDisable( inputdata_t &data )
{
	m_bDisabled = true;
}