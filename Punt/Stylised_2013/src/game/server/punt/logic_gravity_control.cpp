//=========== Copyright © 2012, rHetorical, All rights reserved. =============
//
// Purpose: To control, reset, and send outputs to other entities on the  
// gravity's state.
//		
//=============================================================================

#include "cbase.h"
#include <convar.h>
#include "func_break.h"
#include "engine/ienginesound.h"
#include "vphysics/player_controller.h"
#include "soundent.h"
#include "game.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar is_grav_flipped;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CLogicGravity : public CLogicalEntity
{
public: 	
	
	DECLARE_CLASS( CLogicGravity, CLogicalEntity);

	void Spawn( );
	void Precache( void );
	void Think( void );

	DECLARE_DATADESC();

	// Input functions.
	void InputFlipGravity( inputdata_t &inputData);
	void InputResetGravity( inputdata_t &inputData);
	void InputEnable( inputdata_t &inputdata );
	void InputDisable( inputdata_t &inputdata );
	void InputToggle( inputdata_t &inputdata );

	// Output functions.

private:
	
	bool m_bDisabled;
	COutputEvent	m_OnGravityFlipped;
	COutputEvent	m_OnGravityNormal;

};

LINK_ENTITY_TO_CLASS( logic_gravity_control, CLogicGravity);

BEGIN_DATADESC( CLogicGravity)

	//Save/load
	DEFINE_KEYFIELD(m_bDisabled, FIELD_BOOLEAN, "StartDisabled"),
	
	DEFINE_INPUTFUNC(FIELD_VOID, "Enable", InputEnable),
	DEFINE_INPUTFUNC(FIELD_VOID, "Disable", InputDisable),
	DEFINE_INPUTFUNC(FIELD_VOID, "Toggle", InputToggle),
	DEFINE_INPUTFUNC( FIELD_VOID, "FlipGravity", InputFlipGravity ),
	DEFINE_INPUTFUNC( FIELD_VOID, "ResetGravity", InputResetGravity ),

	DEFINE_OUTPUT( m_OnGravityFlipped, "OnGravityFlipped" ),
	DEFINE_OUTPUT( m_OnGravityNormal, "OnGravityNormal" ),


END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CLogicGravity::Spawn( void )
{
	Precache();
	Think();
	BaseClass::Spawn();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CLogicGravity::Precache( void )
{
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CLogicGravity::Think( void )
{
	SetNextThink( gpGlobals->curtime + .1 );
	if (is_grav_flipped.GetBool())
	{
		m_OnGravityFlipped.FireOutput( this, this );
	}
	else
	{
		m_OnGravityNormal.FireOutput( this, this );
	}
}

//------------------------------------------------------------------------------
// Purpose: Turns on the relay, allowing it to fire outputs.
//------------------------------------------------------------------------------
void CLogicGravity::InputEnable( inputdata_t &inputdata )
{
	m_bDisabled = false;
}

//------------------------------------------------------------------------------
// Purpose: Turns on the relay, allowing it to fire outputs.
//------------------------------------------------------------------------------
void CLogicGravity::InputDisable( inputdata_t &inputdata )
{
	m_bDisabled = true;
}

//------------------------------------------------------------------------------
// Purpose: Turns on the relay, allowing it to fire outputs.
//------------------------------------------------------------------------------
void CLogicGravity::InputToggle( inputdata_t &inputdata )
{
	m_bDisabled = !m_bDisabled;
}

//-----------------------------------------------------------------------------
// Purpose: "Then I flip a switch..."
//-----------------------------------------------------------------------------
void CLogicGravity::InputFlipGravity( inputdata_t &inputData)
{
	if ((!m_bDisabled) && (is_grav_flipped.GetBool()))
	{
		is_grav_flipped.SetValue(0);
	}
	else
	{
		is_grav_flipped.SetValue(1);
	}
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CLogicGravity::InputResetGravity( inputdata_t &inputData)
{
	if ((!m_bDisabled) && (is_grav_flipped.GetBool()))
	{
		is_grav_flipped.SetValue(0);
	}
}
