//=========== Copyright © 2012, rHetorical, All rights reserved. =============
//
// Purpose: 
//		
//=============================================================================
 
#include "cbase.h"
#include <convar.h>
#include "func_break.h"
#include "engine/ienginesound.h"
#include "vphysics/player_controller.h"
#include "soundent.h"
#include "entityinput.h"
#include "entityoutput.h"
#include "eventqueue.h"
#include "mathlib/mathlib.h"
#include "globalstate.h"
#include "ndebugoverlay.h"
#include "saverestore_utlvector.h"
#include "vstdlib/random.h"
#include "gameinterface.h"
#include "game.h"


// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Timer
const int SF_TIMER_UPDOWN = 1;
const float LOGIC_TIMER_MIN_INTERVAL = 0.01;

class CIndicator : public CBaseAnimating
{
public:
	DECLARE_CLASS( CIndicator, CBaseAnimating );
	DECLARE_DATADESC();
 
	CIndicator()
	{
	 m_bActive = false;
	}
 
	void Spawn( void );
	void Precache( void );
	void Think( void );

	// Input functions.
	void InputActivate( inputdata_t &inputData);
//	void InputActivateTime( inputdata_t &inputData);
	void InputDeactivate( inputdata_t &inputData);
	void ResetIndicator ();


private:

	bool	m_bActive;
	bool	m_bisTimed;
	bool	m_bisQuiet;
	float	m_flTimeAmount;

	COutputEvent	m_OnActivate;
	COutputEvent	m_OnDeactivate;

};
 
LINK_ENTITY_TO_CLASS( prop_indicator_panel, CIndicator );
 
// Start of our data description for the class
BEGIN_DATADESC( CIndicator )
 
	//Save/load

	DEFINE_FIELD( m_bisTimed, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flTimeAmount, FIELD_FLOAT ),
	DEFINE_FIELD( m_bisQuiet, FIELD_BOOLEAN ),

	DEFINE_KEYFIELD( m_bisTimed, FIELD_BOOLEAN, "timed" ),
	DEFINE_KEYFIELD( m_flTimeAmount, FIELD_FLOAT, "timeamount" ),
	DEFINE_KEYFIELD( m_bisQuiet, FIELD_BOOLEAN, "quiet" ),

	DEFINE_INPUTFUNC( FIELD_VOID, "Activate", InputActivate ),
//	DEFINE_INPUTFUNC( FIELD_VOID, "ActivateTime", InputActivateTime ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Deactivate", InputDeactivate ),

	DEFINE_OUTPUT( m_OnActivate, "OnActivate" ),
	DEFINE_OUTPUT( m_OnDeactivate, "OnDeactivate" ),
 
END_DATADESC()
 
// Name of our entity's model
//#define	ENTITY_MODEL	"models/props_signage/indicator_panel.mdl" 
#define	ENTITY_MODEL	"models/props_vectech/indicator_panel.mdl" 
 
//-----------------------------------------------------------------------------
// Purpose: Precache assets used by the entity
//-----------------------------------------------------------------------------
void CIndicator::Precache( void )
{
	PrecacheModel( ENTITY_MODEL );

	PrecacheScriptSound( "Punt.TickTock_Loop" );
 
	BaseClass::Precache();
}
 
//-----------------------------------------------------------------------------
// Purpose: Sets up the entity's initial state
//-----------------------------------------------------------------------------
void CIndicator::Spawn( void )
{
	Precache();
 
	SetModel( ENTITY_MODEL );
	SetSolid( SOLID_VPHYSICS );

	AddEffects(EF_NOSHADOW);	

	m_bActive = true;

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CIndicator::Think( void )
{
	BaseClass::Think();
	StopSound( "Punt.TickTock_Loop" );
	ResetIndicator();

	DevMsg("Done Thinking\n");
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &inputdata - 
//-----------------------------------------------------------------------------
void CIndicator::InputActivate( inputdata_t &inputData )
{
	if (!m_bisTimed)
	{
		m_nSkin = 1;
		m_OnActivate.FireOutput( inputData.pActivator, this );
		CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
	
		while( pEntity ) 
		{
			pEntity->SetTextureFrameIndex( 1 );
			pEntity = gEntList.FindEntityByName( pEntity, m_target );  
		}
	}
	else
	{
		m_nSkin = 2;
		m_OnActivate.FireOutput( inputData.pActivator, this );
		CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
		if (!m_bisQuiet)
		{
			EmitSound( "Punt.TickTock_Loop" );
		}
		SetNextThink( gpGlobals->curtime + m_flTimeAmount );
	
		while( pEntity ) 
		{
			pEntity->SetTextureFrameIndex( 2 );
			pEntity = gEntList.FindEntityByName( pEntity, m_target );  
		}
	}
}

void CIndicator::InputDeactivate( inputdata_t &inputData )
{
	m_nSkin = 0;
	m_OnDeactivate.FireOutput( inputData.pActivator, this );
	CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
		
	while( pEntity ) 
	{
		pEntity->SetTextureFrameIndex( 0 );
		pEntity = gEntList.FindEntityByName( pEntity, m_target );  
	}
}

void CIndicator::ResetIndicator()
{
	m_nSkin = 0;
	m_OnDeactivate.FireOutput( this, this );
	CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
		
	while( pEntity ) 
	{
		pEntity->SetTextureFrameIndex( 0 );
		pEntity = gEntList.FindEntityByName( pEntity, m_target );  
	}
}
