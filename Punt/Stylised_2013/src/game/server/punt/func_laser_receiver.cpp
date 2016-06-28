//=========== Copyright © 2012, rHetorical, All rights reserved. =============
//
// Purpose: A brush that sends outputs if it is taking damage, or not.
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

class CFuncLReceiver : public CPointEntity
{
public:
	DECLARE_CLASS( CFuncLReceiver, CPointEntity );
	DECLARE_DATADESC();
 
	void Spawn();
	bool CreateVPhysics( void );
	void Think( void );
	void ThinkHit( void );
	
private:

 	void InputEnable( inputdata_t &data );
	void InputDisable( inputdata_t &data );
	virtual int OnTakeDamage( const CTakeDamageInfo &info );
	COutputEvent	m_OnDamaged;
	COutputEvent	m_OnOffDamaged;
	bool m_bDisabled;
	bool m_TakingDamage;
	bool m_TookDamage;

};

LINK_ENTITY_TO_CLASS( func_laser_receiver, CFuncLReceiver );
 
BEGIN_DATADESC( CFuncLReceiver )

	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),

	DEFINE_KEYFIELD(m_bDisabled, FIELD_BOOLEAN, "StartDisabled"),
	
	DEFINE_INPUTFUNC(FIELD_VOID, "Enable", InputEnable),
	DEFINE_INPUTFUNC(FIELD_VOID, "Disable", InputDisable),

	// Outputs
	DEFINE_OUTPUT( m_OnDamaged, "OnLaserHit" ),
	DEFINE_OUTPUT( m_OnOffDamaged, "OnLaserLost" ),

	DEFINE_THINKFUNC( ThinkHit),
 
END_DATADESC()

bool CFuncLReceiver::CreateVPhysics( void )
{
	VPhysicsInitStatic();
	return true;
}

void CFuncLReceiver::Spawn()
{
	BaseClass::Spawn();
 
	// We should collide with physics
	SetSolid( SOLID_BSP );
	SetCollisionGroup( COLLISION_GROUP_UNPUNTABLE );
 
	// Use our brushmodel
	SetModel( STRING( GetModelName() ) );
 
	// We push things out of our way
	SetMoveType( MOVETYPE_PUSH );
	AddFlag( FL_WORLDBRUSH );

	m_takedamage = DAMAGE_EVENTS_ONLY;

//    SetNextThink( gpGlobals->curtime );

	RegisterThinkContext( "ThinkHit" );
	SetContextThink( &CFuncLReceiver::ThinkHit, gpGlobals->curtime, "ThinkHit" );

	CreateVPhysics();
}

void CFuncLReceiver::Think( void )
{
	if (!m_TakingDamage)
	{
		m_OnOffDamaged.FireOutput( this, this );
		m_TookDamage = false;
	}
}

void CFuncLReceiver::InputEnable( inputdata_t &inputdata )
{
	m_bDisabled = false;
	m_takedamage = DAMAGE_EVENTS_ONLY;
}

void CFuncLReceiver::InputDisable( inputdata_t &inputdata )
{
	m_bDisabled = true;
	m_takedamage = DAMAGE_NO;
}

int CFuncLReceiver::OnTakeDamage( const CTakeDamageInfo &info )
{
	int ret = BaseClass::OnTakeDamage( info );
	Vector	vecTemp;
	CTakeDamageInfo subInfo = info;
	vecTemp = subInfo.GetInflictor()->GetAbsOrigin() - WorldSpaceCenter();

//	if ( info.GetDamageType() & DMG_ENERGYBEAM ) 
	if ( subInfo.GetDamageType() & DMG_ENERGYBEAM )
	{
		if ( info.GetInflictor() )
		{
			if (!m_TookDamage)
			{
				m_OnDamaged.FireOutput( info.GetAttacker(), this );
			}
			// We are taking damage. Lets have our thinker think about this. (lol)

			// We took damage!
			m_TakingDamage = true;
			m_TookDamage = true;

			SetNextThink(gpGlobals->curtime, "ThinkHit");
		}
		else
		{
			m_TakingDamage = false;
		}
	}
	else
	{
		m_TakingDamage = false;
	}

	return ret;
}

void CFuncLReceiver::ThinkHit( void )
{	
	m_TakingDamage = false;
	SetNextThink(gpGlobals->curtime + .1);
}