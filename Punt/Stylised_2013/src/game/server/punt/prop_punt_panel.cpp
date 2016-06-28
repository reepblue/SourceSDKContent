//=========== Copyright © 2012, rHetorical, All rights reserved. =============
//
// Purpose: A prop entity that reads if the player has punted it or not, then
// fires outputs.
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
#include "vphysics/constraints.h"
#include "particle_parse.h"
#include "game.h"

extern ConVar filter_toggle;
extern ConVar filter_player_act;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CPuntIndicator : public CBaseAnimating, public CDefaultPlayerPickupVPhysics
{
public:
	DECLARE_CLASS( CPuntIndicator, CBaseAnimating ); 
	DECLARE_DATADESC();

	bool CreateVPhysics()
	{
		VPhysicsInitNormal( SOLID_VPHYSICS, 0, false );
		VPhysicsGetObject()->EnableMotion(false);
		return true;
	}
 
	CPuntIndicator()
	{
	//	m_intPanelType = 0;
		m_bisToggled = false;
		m_bislocked = false;
		m_bwantstobelocked = false;
	}
 
	void Spawn( void );
	void Precache( void );
	void Think( void );
	void ResetThink( void );
	void Activate( inputdata_t &inputData );
	void CheckLock();

	// Input functions.
	void OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason );
	void ResetIndicator( inputdata_t &inputData );

private:

	bool	m_bisToggled;
	bool    m_bisTimed;
	bool	m_bwantstobelocked;
	bool	m_bislocked;
	float	m_flTimeAmount;
//	int		m_intPanelType;
	CHandle<CBasePlayer>	m_hPhysicsAttacker;

	COutputEvent	m_OnActivate;
	COutputEvent	m_OnDeactivate;

};
 
LINK_ENTITY_TO_CLASS( prop_punt_panel, CPuntIndicator );
 
// Start of our data description for the class
BEGIN_DATADESC( CPuntIndicator )
 
	//Save/load
	DEFINE_FIELD( m_bisTimed, FIELD_BOOLEAN ),
//	DEFINE_KEYFIELD( m_intPanelType, FIELD_INTEGER, "PanelType" ),

	DEFINE_KEYFIELD( m_bisTimed, FIELD_BOOLEAN, "timed" ),
	DEFINE_KEYFIELD( m_flTimeAmount, FIELD_FLOAT, "timeamount" ),
	DEFINE_KEYFIELD( m_bwantstobelocked, FIELD_BOOLEAN, "locked" ),

	DEFINE_OUTPUT( m_OnActivate, "OnActivate" ),
	DEFINE_OUTPUT( m_OnDeactivate, "OnDeactivate" ),

	DEFINE_INPUTFUNC( FIELD_VOID, "Reset", ResetIndicator ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Activate", Activate ),

	// Think functions
	DEFINE_THINKFUNC( ResetThink ),
 
END_DATADESC()
 
// Name of our entity's model
#define	TARGET_MODEL	"models/props_gameplay/punt_panel.mdl"
//#define	ARROW_MODEL		"models/props_signage/punt_panel_arrowdown.mdl"
//#define	ARROWUP_MODEL	"models/props_signage/punt_panel_arrowup.mdl"
 
//-----------------------------------------------------------------------------
// Purpose: Precache assets used by the entity
//-----------------------------------------------------------------------------
void CPuntIndicator::Precache( void )
{
//	if ( m_intPanelType == 0 )
//	{
 		PrecacheModel( TARGET_MODEL );
//	}
//	if ( m_intPanelType == 1 )
//	{
 //		PrecacheModel( ARROW_MODEL );
//	}
//	if ( m_intPanelType == 2 )
//	{
//		PrecacheModel( ARROWUP_MODEL );
//	}
	PrecacheScriptSound( "Punt.TickTock_Loop" );

	PrecacheScriptSound( "Punt.BoxActivate" );
	PrecacheScriptSound( "Punt.BoxDeactivate" );
 
	BaseClass::Precache();
}
 
//-----------------------------------------------------------------------------
// Purpose: Sets up the entity's initial state
//-----------------------------------------------------------------------------
void CPuntIndicator::Spawn( void )
{
	Precache();
//	if ( m_intPanelType == 0 )
//	{
 		SetModel( TARGET_MODEL );
//	}
/*
	if ( m_intPanelType == 1 )
	{
 		SetModel( ARROW_MODEL );
	}
	if ( m_intPanelType == 2 )
	{
		SetModel( ARROWUP_MODEL );
	}
*/
	SetSolid( SOLID_VPHYSICS );
	SetMoveType( MOVETYPE_NONE );
	this->SetCollisionGroup( COLLISION_GROUP_PUNTABLE );

	AddEffects(EF_NOSHADOW);
	m_nSkin = 0;
	CreateVPhysics();
	CheckLock();
	SetNextThink( gpGlobals->curtime );
	RegisterThinkContext( "Timer" );
	SetContextThink( &CPuntIndicator::ResetThink, gpGlobals->curtime, "Timer" );

	BaseClass::Spawn();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPuntIndicator::Think( void )
{
	BaseClass::Think();
	if(!m_bisToggled)
	{
		m_nSkin = 0;
		CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
		
		while( pEntity ) 
		{
			pEntity->SetTextureFrameIndex( 0 );
			pEntity = gEntList.FindEntityByName( pEntity, m_target );  
		}
	}

	if(m_bisTimed)
	{
		m_bislocked = false;

	}

	// The player walked threw a fizzler.
	if (filter_toggle.GetBool() && m_bisToggled )
	{
		filter_player_act.SetValue(1); 
		m_nSkin = 0;
		this->SetCollisionGroup( COLLISION_GROUP_PUNTABLE );
		StopSound( "Punt.TickTock_Loop" );
		EmitSound( "Punt.BoxDeactivate" );
		SetNextThink( NULL );
		m_OnDeactivate.FireOutput( this, this );
		CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
		m_bisToggled = false;

		while( pEntity ) 
		{
			pEntity->SetTextureFrameIndex( 0 );
			pEntity = gEntList.FindEntityByName( pEntity, m_target );  
		}
	}

	// Lets advoid Punting Spam!
	if (filter_toggle.GetBool() && !m_bisToggled )
	{
		SetCollisionGroup( COLLISION_GROUP_UNPUNTABLE );
	}

	if (!filter_toggle.GetBool() && !m_bisToggled )
	{
		SetCollisionGroup( COLLISION_GROUP_PUNTABLE );
	}

	SetNextThink( gpGlobals->curtime );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPuntIndicator::ResetThink( void )
{
	if (m_bisToggled)
	{
		BaseClass::Think();
		StopSound( "Punt.TickTock_Loop" );

		m_nSkin = 0;
		this->SetCollisionGroup( COLLISION_GROUP_PUNTABLE );
		m_OnDeactivate.FireOutput( this, this );
		EmitSound( "Punt.BoxDeactivate" );
		CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
		
		while( pEntity ) 
		{
			pEntity->SetTextureFrameIndex( 0 );
			pEntity = gEntList.FindEntityByName( pEntity, m_target );  
		}
		m_bisToggled = false;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPuntIndicator::OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason )
{

	m_hPhysicsAttacker = pPhysGunUser;

	if( reason == PUNTED_BY_CANNON )
	{
			CheckLock();
			if (!m_bisToggled )
			{
			EmitSound( "Punt.BoxActivate" );
			this->SetCollisionGroup( COLLISION_GROUP_UNPUNTABLE );
//			ConVar *puntbox_show_particles = cvar->FindVar( "puntbox_show_particles" );
//			if ( puntbox_show_particles->GetBool() )
//			{
//				DispatchParticleEffect ("portal_1_badvolume_", GetAbsOrigin(), QAngle( 0, 0, 0 ));
//			}
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
				this->SetCollisionGroup( COLLISION_GROUP_UNPUNTABLE );
				EmitSound( "Punt.TickTock_Loop" );
				m_OnActivate.FireOutput( this, this );
				CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
				SetNextThink(gpGlobals->curtime + m_flTimeAmount, "Timer" );
			
				while( pEntity ) 
				{
					pEntity->SetTextureFrameIndex( 2 );
					pEntity = gEntList.FindEntityByName( pEntity, m_target );  
				}
			}
				m_bisToggled = true;
			}
			else 
			{	
				if (!m_bislocked)
				{
					m_nSkin = 0;
					this->SetCollisionGroup( COLLISION_GROUP_PUNTABLE );
					m_OnDeactivate.FireOutput( this, this );
					EmitSound( "Punt.BoxDeactivate" );
					//ConVar *puntbox_show_particles = cvar->FindVar( "puntbox_show_particles" );
					//		if ( puntbox_show_particles->GetBool() )
					//		{
					//			DispatchParticleEffect ("portal_1_badvolume_", GetAbsOrigin(), QAngle( 0, 0, 0 ));
					//		}
					CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
	
					while( pEntity ) 
					{
						pEntity->SetTextureFrameIndex( 0 );
						pEntity = gEntList.FindEntityByName( pEntity, m_target );  
					}
					m_bisToggled = false;
				}
			}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &inputdata - 
//-----------------------------------------------------------------------------
void CPuntIndicator::CheckLock()
{
	if (m_bwantstobelocked)
	{
		m_bislocked = true;
	}

}

void CPuntIndicator::ResetIndicator( inputdata_t &inputData )
{
	m_nSkin = 0;
	this->SetCollisionGroup( COLLISION_GROUP_PUNTABLE );
	EmitSound( "Punt.BoxDeactivate" );
	m_OnDeactivate.FireOutput( this, this );
	CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
	m_bisToggled = false;
	m_bisToggled = false;

	while( pEntity ) 
	{
		pEntity->SetTextureFrameIndex( 0 );
		pEntity = gEntList.FindEntityByName( pEntity, m_target );  
	}
}

void CPuntIndicator::Activate( inputdata_t &inputData )
{
	CheckLock();
	if (!m_bisToggled )
	{
		EmitSound( "Punt.BoxActivate" );
		this->SetCollisionGroup( COLLISION_GROUP_UNPUNTABLE );
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
				this->SetCollisionGroup( COLLISION_GROUP_UNPUNTABLE );
				EmitSound( "Punt.TickTock_Loop" );
				m_OnActivate.FireOutput( inputData.pActivator, this );
				CBaseEntity *pEntity = gEntList.FindEntityByName( NULL, m_target );
				SetNextThink(gpGlobals->curtime + m_flTimeAmount, "Timer" );
			
				while( pEntity ) 
				{
					pEntity->SetTextureFrameIndex( 2 );
					pEntity = gEntList.FindEntityByName( pEntity, m_target );  
				}
			}
				m_bisToggled = true;
	}
}