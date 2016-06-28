//=========== Copyright © 2014, rHetorical, All rights reserved. =============
//
// Purpose: 
//		
//=============================================================================

#include "cbase.h"
#include "baseglowanimating.h"

LINK_ENTITY_TO_CLASS( prop_glow, CBaseGlowAnimating );
PRECACHE_REGISTER(prop_glow);

IMPLEMENT_SERVERCLASS_ST(CBaseGlowAnimating, DT_BaseGlowAnimating)
	SendPropBool( SENDINFO( m_bGlowEnabled ) ),
	SendPropFloat( SENDINFO( m_flRedGlowColor ) ),
	SendPropFloat( SENDINFO( m_flGreenGlowColor ) ),
	SendPropFloat( SENDINFO( m_flBlueGlowColor ) ),
END_SEND_TABLE()

BEGIN_DATADESC( CBaseGlowAnimating )

	//Save/load

	DEFINE_FIELD( m_bGlowEnabled, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flRedGlowColor, FIELD_FLOAT ),
	DEFINE_FIELD( m_flGreenGlowColor, FIELD_FLOAT ),
	DEFINE_FIELD( m_flBlueGlowColor, FIELD_FLOAT ),


	// I/O
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableGlow", InputStartGlow ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisableGlow", InputEndGlow ),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CBaseGlowAnimating::CBaseGlowAnimating()
{
	m_bGlowEnabled.Set( false );

	m_flRedGlowColor = 0.76f;
	m_flGreenGlowColor = 0.76f;
	m_flBlueGlowColor = 0.76f;

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseGlowAnimating::Precache( void )
{
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseGlowAnimating::Spawn( void )
{
	BaseClass::Spawn();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseGlowAnimating::AddGlowEffect( void )
{
	SetTransmitState( FL_EDICT_ALWAYS );
	m_bGlowEnabled.Set( true );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseGlowAnimating::RemoveGlowEffect( void )
{
	m_bGlowEnabled.Set( false );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CBaseGlowAnimating::IsGlowEffectActive( void )
{
	return m_bGlowEnabled;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseGlowAnimating::SetGlowVector(float r, float g, float b )
{
	m_flRedGlowColor = r;
	m_flGreenGlowColor = g;
	m_flBlueGlowColor = b;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseGlowAnimating::InputStartGlow( inputdata_t &inputData )
{
	AddGlowEffect();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseGlowAnimating::InputEndGlow( inputdata_t &inputData )
{
	RemoveGlowEffect();
}