//=========== Copyright © 2012, rHetorical, All rights reserved. =============
//
// Purpose: Punt's box.
//		
//=============================================================================

#include "cbase.h"
#include "engine/IEngineSound.h"
#include "locksounds.h"
#include "filters.h"
#include "physics.h"
#include "vphysics_interface.h"
#include "entityoutput.h"
#include "vcollide_parse.h"
#include "studio.h"
#include "explode.h"
#include "utlrbtree.h"
#include "tier1/strtools.h"
#include "physics_impact_damage.h"
#include "KeyValues.h"
#include "filesystem.h"
#include "scriptevent.h"
#include "entityblocker.h"
#include "soundent.h"
#include "EntityFlame.h"
#include "game.h"
#include "physics_prop_ragdoll.h"
#include "decals.h"
#include "hierarchy.h"
#include "shareddefs.h"
#include "physobj.h"
#include "physics_npc_solver.h"
#include "SoundEmitterSystem/isoundemittersystembase.h"
#include "datacache/imdlcache.h"
#include "props.h"
#include "physics_collisionevent.h"
#include "GameStats.h"
//#include "EnvLaser.h"
#include "particle_parse.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Here we list convars for particles and player effects.

// Umm, there was this cube that changed the players walk speed. 
// Keeping it here just in case we need it again.
//ConVar punt_fastspeed( "punt_fastspeed", "1200" );

// Particles. Some people may dislike the particle effects.
ConVar puntbox_show_particles( "puntbox_show_particles", "1" );

// Tells the world "Hey, the phys gravity is flipped!"
ConVar is_grav_flipped( "is_grav_flipped", "0", FCVAR_HIDDEN, "Is the gravity flipped?" );

// Here are somethings for punt fizzling.
extern ConVar filter_toggle;
extern ConVar filter_player_act;
extern ConVar w_season;

// Models
#define PUNTBOX_MODEL "models/props_gameplay/puntbox.mdl"
#define ICEBOX_MODEL "models/props_gameplay/puntbox_ice.mdl"

//-----------------------------------------------------------------------------
// Purpose: Base
//-----------------------------------------------------------------------------
class CPuntBox : public CBaseAnimating, public CDefaultPlayerPickupVPhysics
{
public:
	DECLARE_CLASS( CPuntBox, CBaseAnimating );
	DECLARE_DATADESC();

	bool CreateVPhysics()
	{
		VPhysicsInitNormal( SOLID_VPHYSICS, 0, false );
		return true;
	}

	//Constructor
	CPuntBox()
	{
		m_intBoxType = 0;		//Box Type
		m_bisToggled = false;	 // Are we activated?
		m_bReady = true;	// Enable punting/lifting
	}

	void Spawn( void );
	void Precache( void );
//	void Think( void );
	// We NEED this context think. If we place the functions in the normal Think, it will not work!
	void CleanserThink ( void ); 
	void MakeParticle ( void );

	void Pickup( void );
	void OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason );
	void OnPhysGunDrop( CBasePlayer *pPhysGunUser, PhysGunDrop_t reason );
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	int ObjectCaps();

	// Time Box
	void ResetBox();

	// Bomb 
	void OnExplode();



	// Base Activations
	void ActivateFlip();
	void ActivateTime();
	void DeactivateFlip();
	void DeactivateTime();

	// Inputs
	void ActivateCube( inputdata_t &inputData );
	void DeactivateCube( inputdata_t &inputData );
	void InputDissolve( inputdata_t &inputData );
	void InputSilentDissolve( inputdata_t &inputData );
	void InputEnablePunting ( inputdata_t &inputData );
	void InputDisablePunting ( inputdata_t &inputData );

private:

	bool	m_bisToggled;
	bool	m_bReady;
	int		m_intBoxType;

	// Bomb Box
	bool	m_bGib;      

	CHandle<CBasePlayer>	m_hPhysicsAttacker;
	
	Vector MdlTop;

	COutputEvent m_OnPlayerUse;
	COutputEvent m_OnLaserPunt;
	COutputEvent m_OffLaserPunt;
	COutputEvent m_OnPhysGunPunt;
	COutputEvent m_OffPhysGunPunt;
	COutputEvent m_OnPlayerPickup;
	COutputEvent m_OnPhysGunPickup;
	COutputEvent m_OnPhysGunDrop;

	// Bomb
	COutputEvent m_OnExplode;

	// For Flip Box
	Vector	newgravity;
};

LINK_ENTITY_TO_CLASS( prop_punt_box, CPuntBox );
PRECACHE_REGISTER(prop_punt_box);

BEGIN_DATADESC( CPuntBox )

	//Save/load
	DEFINE_USEFUNC( Use ),
	DEFINE_FIELD( m_bisToggled, FIELD_BOOLEAN ),
	DEFINE_KEYFIELD( m_intBoxType, FIELD_INTEGER, "BoxType" ),
	DEFINE_KEYFIELD( m_bReady, FIELD_BOOLEAN, "enablepunt" ),

	//I/O
	DEFINE_INPUTFUNC( FIELD_VOID, "Activate", ActivateCube ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Deactivate", DeactivateCube ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Dissolve", InputDissolve ),
	DEFINE_INPUTFUNC( FIELD_VOID, "SilentDissolve", InputSilentDissolve ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnablePunt", InputEnablePunting ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisablePunt", InputDisablePunting ),

	DEFINE_OUTPUT( m_OnExplode, "OnExplode" ),
	DEFINE_OUTPUT ( m_OnPhysGunPunt, "OnPhysGunPunt" ),
	DEFINE_OUTPUT ( m_OffPhysGunPunt, "OffPhysGunPunt" ),
	DEFINE_OUTPUT( m_OnPhysGunDrop, "OnPhysGunDrop" ),
	DEFINE_OUTPUT ( m_OnLaserPunt, "OnLaserPunt" ),
	DEFINE_OUTPUT ( m_OffLaserPunt, "OffLaserPunt" ),
	DEFINE_OUTPUT( m_OnPlayerUse, "OnPlayerUse" ),
	DEFINE_OUTPUT( m_OnPlayerPickup, "OnPlayerPickup" ),
	DEFINE_OUTPUT( m_OnPhysGunPickup, "OnPhysGunPickup" ),

	// Think functions
	DEFINE_THINKFUNC( CleanserThink),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPuntBox::Precache( void )
{
	// ALL Boxes share one model!
	PrecacheModel( PUNTBOX_MODEL );
	PrecacheModel( ICEBOX_MODEL );

	// Precache sounds/particles
	PrecacheParticleSystem ("punt_efx_hit-on");
	PrecacheParticleSystem ("punt_efx_hit-off");
	PrecacheParticleSystem ("punt_efx_hit-bomb"); 
	PrecacheParticleSystem ("icebox_smoke");

	PrecacheScriptSound( "Punt.BoxActivate" );
	PrecacheScriptSound( "Punt.BoxDeactivate" );
	PrecacheScriptSound( "Punt.Fizzle" );

	BaseClass::Precache();
}
//-----------------------------------------------------------------------------
// Purpose: Spawn the box into the world!
//-----------------------------------------------------------------------------
void CPuntBox::Spawn( void )
{
	// Again, all boxes share the same model. However, during Winter, the model is changed due to properties.

	// In Winter, the boxes are all icey and slip!
	if ( w_season.GetInt() == 3 )
	{
		SetModel( ICEBOX_MODEL );
		MakeParticle();
	}
	else
	{
		SetModel( PUNTBOX_MODEL );
	}
	Think();

	// The skins however, are diffrent.
	// Normal takes skin 0
	// Flip takes skins 1 and 2
	// Time takes skins 3 and 4
	// Bomb takes skin 5
	
	// Normal
	if ( m_intBoxType == 0 )
	{
		m_nSkin = 0;
		SetCollisionGroup( COLLISION_GROUP_UNPUNTABLE );
	}

	// Flip
	if ( m_intBoxType == 1 )
	{
		m_nSkin = 1;
		SetCollisionGroup( COLLISION_GROUP_PUNTABLE );
	}

	// Timed
	if ( m_intBoxType == 2 )
	{
		m_nSkin = 3;
		SetCollisionGroup( COLLISION_GROUP_PUNTABLE );
	}

	// Bomb
	if ( m_intBoxType == 3 )
	{
		m_nSkin = 5;
		SetCollisionGroup( COLLISION_GROUP_PUNTABLE );
	}

	SetSolid( SOLID_VPHYSICS );
	BaseClass::Spawn();
	CreateVPhysics();
	SetUse( &CPuntBox::Use );
	RegisterThinkContext( "CleanserThink" );
	SetContextThink( &CPuntBox::CleanserThink, gpGlobals->curtime, "CleanserThink" );
	SetNextThink(gpGlobals->curtime + .1, "CleanserThink" );

	m_bisToggled = false;

}
void CPuntBox::MakeParticle( void )
{
	//Bug: Box will not spawn a particle unless it is spawned with a template.
	DevMsg ("Spawned Particle\n");
	DispatchParticleEffect ("icebox_smoke", PATTACH_ABSORIGIN_FOLLOW, this );
}

int CPuntBox::ObjectCaps()
{ 
	int caps = BaseClass::ObjectCaps();

	if ( CBasePlayer::CanPickupObject( this, 45, 128 ) )
	{
		caps |= FCAP_IMPULSE_USE;
	}

	return caps;
}
void CPuntBox::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	DevMsg ("+USE WAS PRESSED ON ME\n");
	CBasePlayer *pPlayer = ToBasePlayer( pActivator );
	if ( pPlayer )
	{
		if ( HasSpawnFlags( SF_PHYSPROP_ENABLE_PICKUP_OUTPUT ) )
		{
			m_OnPlayerUse.FireOutput( this, this );
		}
		ConVar *c_puntgun_highlight = cvar->FindVar( "c_puntgun_highlight" );
		c_puntgun_highlight->SetValue(0);
		pPlayer->PickupObject( this );
	}

	// Pass up so we still call any custom Use function
//	BaseClass::Use( pActivator, pCaller, useType, value );
}
void CPuntBox::CleanserThink( void )
{
	ConVar *is_grav_flipped = cvar->FindVar( "is_grav_flipped" );
	if (is_grav_flipped->GetBool())
	{
		if ( m_intBoxType != 2 || !m_bisToggled )
		{
			VPhysicsGetObject()->Wake();
		}
		else if ( m_intBoxType == 2 )
		{
			VPhysicsGetObject()->Wake();
		}
	}
	SetNextThink(gpGlobals->curtime + .1, "CleanserThink" );
	// The player walked through a fizzler.
	if (filter_toggle.GetBool() && m_bisToggled )
	{
		filter_player_act.SetValue(1);
		m_OffPhysGunPunt.FireOutput( this, this );
		EmitSound( "Punt.BoxDeactivate" );
		// Flip
		if ( m_intBoxType == 1 )
		{
			DeactivateFlip();
			if ( puntbox_show_particles.GetBool() )
			{
				DispatchParticleEffect ("punt_efx_hit-off", GetAbsOrigin(), QAngle( 0, 0, 0 ));
			}
		}
		// Timed
		if ( m_intBoxType == 2 )
		{
			DeactivateTime();
			VPhysicsGetObject()->Wake();
			if ( puntbox_show_particles.GetBool() )
			{
				DispatchParticleEffect ("punt_efx_hit-off", GetAbsOrigin(), QAngle( 0, 0, 0 ));
			}
		}

		m_bisToggled = false;	
	}

	// Lets advoid Punting Spam!
	if (filter_toggle.GetBool() && !m_bisToggled )
	{
		SetCollisionGroup( COLLISION_GROUP_UNPUNTABLE );
	}

	if (!filter_toggle.GetBool() && !m_bisToggled )
	{
		if ( m_intBoxType != 0 )
		{
			SetCollisionGroup( COLLISION_GROUP_PUNTABLE );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: To allow punting or not.
//-----------------------------------------------------------------------------
void CPuntBox::InputEnablePunting ( inputdata_t &inputData )
{
	if ( m_intBoxType != 0 )
	{
		SetCollisionGroup( COLLISION_GROUP_PUNTABLE );
	}
	m_bReady = true;
}

void CPuntBox::InputDisablePunting ( inputdata_t &inputData )
{
	SetCollisionGroup( COLLISION_GROUP_NONE );
	m_bReady = false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPuntBox::OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason )
{
	m_hPhysicsAttacker = pPhysGunUser;

	if( reason == PUNTED_BY_CANNON )
	{
			if (!m_bisToggled)
			{
				m_OnPhysGunPunt.FireOutput( pPhysGunUser, this );

				if ( m_intBoxType == 0 )
				{
					return;
				}
				if ( puntbox_show_particles.GetBool() )
				{
					if (m_intBoxType == 3)
					{
						DispatchParticleEffect ("punt_efx_hit-bomb", GetAbsOrigin(), QAngle( 0, 0, 0 ));
					}
					else
					{
						DispatchParticleEffect ("punt_efx_hit-on", GetAbsOrigin(), QAngle( 0, 0, 0 ));
					}
				}
				EmitSound( "Punt.BoxActivate" );
				m_bisToggled = true;

				// We skip normal box functions.

				// Flip
				if ( m_intBoxType == 1 )
				{
					ActivateFlip();
				}
	
				// Timed
				if ( m_intBoxType == 2 )
				{
					ActivateTime();
				}

				// Bomb
				if ( m_intBoxType == 3 ) 
				{
					OnExplode();
				}
			}
			else
			{	
				m_OffPhysGunPunt.FireOutput( pPhysGunUser, this );
				if ( puntbox_show_particles.GetBool() )
				{
					DispatchParticleEffect ("punt_efx_hit-off", GetAbsOrigin(), QAngle( 0, 0, 0 ));
				}
				EmitSound( "Punt.BoxDeactivate" );
				m_bisToggled = false;

				// Flip
				if ( m_intBoxType == 1 )
				{
					DeactivateFlip();
				}

				// Timed
				if ( m_intBoxType == 2 )
				{
					DeactivateTime();
				}
			}
	}

	if ( reason == PICKED_UP_BY_CANNON || reason == PICKED_UP_BY_PLAYER )
	{
		m_OnPlayerPickup.FireOutput( pPhysGunUser, this );
	}

	if ( reason == PICKED_UP_BY_CANNON )
	{
		m_OnPhysGunPickup.FireOutput( pPhysGunUser, this );
	}
}
void CPuntBox::OnPhysGunDrop( CBasePlayer *pPhysGunUser, PhysGunDrop_t reason )
{
	m_OnPhysGunDrop.FireOutput( pPhysGunUser, this );
	DevMsg ("Player Dropped me\n");
}
//----------------------------------------------------------------------------
// Purpose: Inputs to activate/Deactivate Cubes!
//-----------------------------------------------------------------------------
void CPuntBox::ActivateCube( inputdata_t &inputData )
{
	// Flip
	if ( m_intBoxType == 1 )
	{
		EmitSound( "Punt.BoxActivate" );
		ActivateFlip();
	}

	// Timed
	if ( m_intBoxType == 2 )
	{
		EmitSound( "Punt.BoxActivate" );
		ActivateTime();
	}

	// Bomb
	if ( m_intBoxType == 3 )
	{
		OnExplode();
	}
}

void CPuntBox::DeactivateCube( inputdata_t &inputData )
{
	// Flip
	if ( m_intBoxType == 1 )
	{
		EmitSound( "Punt.BoxDeactivate" );
		DeactivateFlip();
	}

	// Timed
	if ( m_intBoxType == 2 )
	{
		EmitSound( "Punt.BoxDeactivate" );
		DeactivateTime();
	}
}

//-----------------------------------------------------------------------------
// Purpose: FLIP Box - Flips world gravity!
//-----------------------------------------------------------------------------
void CPuntBox::ActivateFlip()
{
	if  ( m_bReady )
	{
		m_nSkin = 2;
		newgravity = Vector(0,0,600);
		physenv->SetGravity(newgravity);
		ConVar *is_grav_flipped = cvar->FindVar( "is_grav_flipped" );
		is_grav_flipped->SetValue(1);
		SetNextThink( NULL );
	}
}
void CPuntBox::DeactivateFlip()
{
	m_nSkin = 1;
	newgravity = Vector(0,0,-600);
	physenv->SetGravity(newgravity);
	//SetAbsVelocity( newgravity );
	ConVar *is_grav_flipped = cvar->FindVar( "is_grav_flipped" );
	is_grav_flipped->SetValue(0);
	SetNextThink(gpGlobals->curtime + .1, "TestContext" );
}

//-----------------------------------------------------------------------------
// Purpose: TIME Box - PAUSES IN MID AIR!
//-----------------------------------------------------------------------------
void CPuntBox::ActivateTime()
{
	if  ( m_bReady )
	{
		m_nSkin = 4;
		VPhysicsGetObject()->EnableMotion ( false );
		SetNextThink( NULL );
	}
}

void CPuntBox::DeactivateTime()
{
	m_nSkin = 3;
	VPhysicsGetObject()->EnableMotion ( true );
	SetNextThink(gpGlobals->curtime + .1, "TestContext" );
}

//-----------------------------------------------------------------------------
// Purpose: BOMB Box - ASPLODE!
//-----------------------------------------------------------------------------
void CPuntBox::OnExplode()
{
	Vector vecUp;
	GetVectors( NULL, NULL, &vecUp );
	Vector vecOrigin = WorldSpaceCenter() + ( vecUp * 12.0f );

	ExplosionCreate( GetAbsOrigin(), GetAbsAngles(), this, 150.0, 125.0, true);
	m_OnExplode.FireOutput( this, this );
	m_bGib = true;

	CPVSFilter filter( vecOrigin );
	for ( int i = 0; i < 4; i++ )
	{
		Vector gibVelocity = RandomVector(-100,100);
		int iModelIndex = modelinfo->GetModelIndex( g_PropDataSystem.GetRandomChunkModel( "MetalChunks" ) );	
		te->BreakModel( filter, 0.0, vecOrigin, GetAbsAngles(), Vector(40,40,40), gibVelocity, iModelIndex, 150, 4, 2.5, BREAK_METAL );
	}
	UTIL_Remove( this );
}

//-----------------------------------------------------------------------------
// Purpose: Dissolve
//-----------------------------------------------------------------------------
void CPuntBox::InputDissolve( inputdata_t &inputData )
{
	// All Share
	this->Dissolve( NULL, gpGlobals->curtime, false, 0 );
	m_nSkin = 0;

	if ( m_intBoxType == 1 )
	{
		newgravity = Vector(0,0,-600);
		physenv->SetGravity(newgravity);
		ConVar *is_grav_flipped = cvar->FindVar( "is_grav_flipped" );
		is_grav_flipped->SetValue(0);
		m_bisToggled = false;
		this->Dissolve( NULL, gpGlobals->curtime, false, 0 );
	}
}

void CPuntBox::InputSilentDissolve( inputdata_t &inputData )
{
	// All Share
	this->Dissolve( NULL, gpGlobals->curtime, false, 0 );
	StopSound( "Punt.Fizzle" );
	m_nSkin = 0;

	if ( m_intBoxType == 1 )
	{
		newgravity = Vector(0,0,-600);
		physenv->SetGravity(newgravity);
		ConVar *is_grav_flipped = cvar->FindVar( "is_grav_flipped" );
		is_grav_flipped->SetValue(0);
		m_bisToggled = false;
		this->Dissolve( NULL, gpGlobals->curtime, false, 0 );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Covar Spawn
//-----------------------------------------------------------------------------

void CC_GivePuntBox( void )
{
	engine->ServerCommand("ent_create prop_punt_box\n");
}

static ConCommand makebox("ent_create_puntbox", CC_GivePuntBox, "Give the player a Puntbox\n", FCVAR_CHEAT );