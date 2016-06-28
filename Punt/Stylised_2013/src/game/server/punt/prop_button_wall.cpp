//=========== Copyright © 2012, rHetorical, All rights reserved. =============
//
// Purpose: Wall button. 
//		
//=============================================================================

#include "cbase.h"
#include "engine/ienginesound.h"
#include "soundent.h"
#include "player_pickup.h"
#include "game.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define BUTTON_MODEL "models/props_gameplay/wall_button.mdl"

//-----------------------------------------------------------------------------
// Purpose: Base
//-----------------------------------------------------------------------------
class CPropWButton : public CBaseAnimating //public CDefaultPlayerPickupVPhysics
{
public:
	DECLARE_CLASS( CPropWButton, CBaseAnimating );
	DECLARE_DATADESC();

	//Constructor
	CPropWButton()
	{
		m_bActive = false;
	}

	void Spawn( void );
	void Precache( void );
	void Think( void );

	void Press();
	void Release();
	void Animate();

	// Input functions.
	void InputActivate( inputdata_t &inputData);
	void InputDeactivate( inputdata_t &inputData);

	//Use
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	int ObjectCaps();

	private:

	bool	m_bActive;
	bool	m_bisTimed;
	float	m_flTimeAmount;

	COutputEvent	m_OnActivate;
	COutputEvent	m_OnDeactivate;

//	CHandle<CBasePlayer>	m_hPhysicsAttacker;

};

LINK_ENTITY_TO_CLASS( prop_button_wall, CPropWButton );
 
// Start of our data description for the class
BEGIN_DATADESC( CPropWButton )
 
	//Save/load

	DEFINE_USEFUNC( Use ),
	DEFINE_FIELD( m_bisTimed, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flTimeAmount, FIELD_FLOAT ),

	DEFINE_KEYFIELD( m_bisTimed, FIELD_BOOLEAN, "timed" ),
	DEFINE_KEYFIELD( m_flTimeAmount, FIELD_FLOAT, "timeamount" ),

	DEFINE_INPUTFUNC( FIELD_VOID, "Press", InputActivate ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Release", InputDeactivate ),

	DEFINE_OUTPUT( m_OnActivate, "OnPressed" ),
	DEFINE_OUTPUT( m_OnDeactivate, "OnReleased" ),
 
END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: Precache assets used by the entity
//-----------------------------------------------------------------------------
void CPropWButton::Precache( void )
{
	PrecacheModel( BUTTON_MODEL );

	PrecacheScriptSound( "Punt.TickTock_Loop" );

	PrecacheScriptSound( "Punt.SwitchDown" );
	PrecacheScriptSound( "Punt.SwitchUp" );
 
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: Sets up the entity's initial state
//-----------------------------------------------------------------------------
void CPropWButton::Spawn( void )
{
	Precache();
	SetModel( BUTTON_MODEL );
	SetSolid( SOLID_BBOX );
	UTIL_SetSize( this, -Vector(20,20,20), Vector(20,20,20) );

	SetUse( &CPropWButton::Use );
	SetSequence( LookupSequence("BindPose") );
	SetCollisionGroup( COLLISION_GROUP_UNPUNTABLE );

	AddEffects(EF_NOSHADOW);	

	m_bActive = false;

	BaseClass::Spawn();

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPropWButton::Think( void )
{
	BaseClass::Think();
	StopSound( "Punt.TickTock_Loop" );
	Release();

	DevMsg("Done Thinking\n");
}


//Here we link USE to the entity
int CPropWButton::ObjectCaps()
{ 
	int caps = BaseClass::ObjectCaps();

	caps |= FCAP_IMPULSE_USE;

	return caps;
}

void CPropWButton::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	DevMsg ("+USE WAS PRESSED ON ME\n");
	CBasePlayer *pPlayer = ToBasePlayer( pActivator );
	if ( pPlayer )
	{
		if (!m_bActive)
		{
			Press();
		}
	}
}

void CPropWButton::Press()
{
	m_bActive = true;
	EmitSound( "Punt.SwitchDown" );
	Animate();
	if (!m_bisTimed)
	{
		m_nSkin = 1;
		m_OnActivate.FireOutput( this, this );
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
		m_OnActivate.FireOutput( this, this );
		CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
		EmitSound( "Punt.TickTock_Loop" );
		SetNextThink( gpGlobals->curtime + m_flTimeAmount );
	
		while( pEntity ) 
		{
			pEntity->SetTextureFrameIndex( 2 );
			pEntity = gEntList.FindEntityByName( pEntity, m_target );  
		}
	}
}
void CPropWButton::Release()
{
	m_bActive = false;
	EmitSound( "Punt.SwitchUp" );
	Animate();
	m_nSkin = 0;
	m_OnDeactivate.FireOutput( this, this );
	CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
		
	while( pEntity ) 
	{
		pEntity->SetTextureFrameIndex( 0 );
		pEntity = gEntList.FindEntityByName( pEntity, m_target );  
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &inputdata - 
//-----------------------------------------------------------------------------
void CPropWButton::InputActivate( inputdata_t &inputData )
{
	m_bActive = true;
	EmitSound( "Punt.SwitchDown" );
	Animate();
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
		EmitSound( "Punt.TickTock_Loop" );
		SetNextThink( gpGlobals->curtime + m_flTimeAmount );
	
		while( pEntity ) 
		{
			pEntity->SetTextureFrameIndex( 2 );
			pEntity = gEntList.FindEntityByName( pEntity, m_target );  
		}
	}
}

void CPropWButton::InputDeactivate( inputdata_t &inputData )
{
	m_bActive = false;
	EmitSound( "Punt.SwitchUp" );
	Animate();
	m_nSkin = 0;
	m_OnDeactivate.FireOutput( inputData.pActivator, this );
	CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
		
	while( pEntity ) 
	{
		pEntity->SetTextureFrameIndex( 0 );
		pEntity = gEntList.FindEntityByName( pEntity, m_target );  
	}
}
void CPropWButton::Animate()
{
	if(!m_bActive)
	{
		// Select the scanner's idle sequence
		SetSequence( LookupSequence("press") );
		// Set the animation speed to 100%
		SetPlaybackRate( 1.0f );
		// Tell the client to animate this model
		UseClientSideAnimation();
	}
	else
	{
		// Select the scanner's idle sequence
		SetSequence( LookupSequence("release") );
		// Set the animation speed to 100%
		SetPlaybackRate( 1.0f );
		// Tell the client to animate this model
		UseClientSideAnimation();
	}
}
