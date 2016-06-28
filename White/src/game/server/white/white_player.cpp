//=========== Copyright © 2013, rHetorical, All rights reserved. =============
//
// Purpose: A Player that is on top of HL2_player. Replace "rHetotical" with
// The current name. (e.g: CrHetoricalPlayer should be CPuntPlayer for PUNT.)
//		
//=============================================================================

#include "cbase.h"
#include "white_player.h" //Your Player file
#include "predicted_viewmodel.h"
#include "player.h"
#include "simtimer.h"
#include "player_pickup.h"
#include "game.h"
#include "gamerules.h"
#include "trains.h"
#include "in_buttons.h" 
#include "globalstate.h"
#include "KeyValues.h"
#include "eventqueue.h"
#include "engine/IEngineSound.h"
#include "ai_basenpc.h"
#include "SoundEmitterSystem/isoundemittersystembase.h"
#include "vphysics/player_controller.h"
#include "datacache/imdlcache.h"
#include "soundenvelope.h"
#include "ai_speech.h"	
#include "sceneentity.h"

// Link us!
LINK_ENTITY_TO_CLASS( player, CrHetoricalPlayer );

//Convars
ConVar player_walkspeed( "player_walkspeed", "145" );
ConVar sv_regeneration_wait_time ("sv_regeneration_wait_time", "1.0", FCVAR_REPLICATED );

//Defines
#define PLAYER_WALK_SPEED player_walkspeed.GetFloat()
#define PLAYER_MAX_LIFT_MASS 85
#define PLAYER_MAX_LIFT_SIZE 128

IMPLEMENT_SERVERCLASS_ST(CrHetoricalPlayer, DT_rHetoricalPlayer)

END_SEND_TABLE()

BEGIN_DATADESC( CrHetoricalPlayer )

	DEFINE_FIELD( m_fTimeLastHurt, FIELD_TIME )

END_DATADESC()

//CBaseEntity *FindEntityForward( CBasePlayer *pMe, bool fHull );

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------
// Basics
//-----------------------
void CrHetoricalPlayer::Precache( void )
{
	BaseClass::Precache();
/*
	PrecacheScriptSound( "Player.UseDeny" );
	PrecacheScriptSound( "Player.PickupWeapon" );
	PrecacheScriptSound( "Player.TrainUse" );
	PrecacheScriptSound( "Player.Use" );
	PrecacheScriptSound( "Player.BurnPain" );
*/
	PrecacheModel ( "models/player.mdl" );
}

void CrHetoricalPlayer::Spawn()
{
	// Dying without a player model crashes the client
	SetModel("models/player.mdl");
	SetMaxSpeed( PLAYER_WALK_SPEED );
	BaseClass::Spawn();
	StartWalking();

	// Give the player the punting device when they spawn
	GiveAllItems();
}

void CrHetoricalPlayer::PostThink()
{
	BaseClass::PostThink();

	// Keep the model upright; pose params will handle pitch aiming.
	QAngle angles = GetLocalAngles();
	angles[PITCH] = 0;
	SetLocalAngles(angles);

	// Regenerate heath after 3 seconds
	if ( IsAlive() && GetHealth() < GetMaxHealth() )
	{
		// Color to overlay on the screen while the player is taking damage
		color32 hurtScreenOverlay = {64,0,0,64};

		if ( gpGlobals->curtime > m_fTimeLastHurt + sv_regeneration_wait_time.GetFloat() )
		{
			TakeHealth( 1, DMG_GENERIC );
			m_bIsRegenerating = true;

			if ( GetHealth() >= GetMaxHealth() )
			{
				m_bIsRegenerating = false;
			}
		}
		else
		{
			m_bIsRegenerating = false;
			UTIL_ScreenFade( this, hurtScreenOverlay, 1.0f, 0.1f, FFADE_IN|FFADE_PURGE );
		}
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------
// Viewmodel and weapon stuff!
//-----------------------
void CrHetoricalPlayer::CreateViewModel( int index )
{
	BaseClass::CreateViewModel( index );
	return;
	Assert( index >= 0 && index < MAX_VIEWMODELS );

	if ( GetViewModel( index ) )
		return;

	CPredictedViewModel *vm = ( CPredictedViewModel * )CreateEntityByName( "predicted_viewmodel" );
	if ( vm )
	{
		vm->SetAbsOrigin( GetAbsOrigin() );
		vm->SetOwner( this );
		vm->SetIndex( index );
		DispatchSpawn( vm );
		vm->FollowEntity( this, false );
		m_hViewModel.Set( index, vm );
	}
}

bool CrHetoricalPlayer::Weapon_Switch( CBaseCombatWeapon *pWeapon, int viewmodelindex )
{
	bool bRet = BaseClass::Weapon_Switch( pWeapon, viewmodelindex );

	return bRet;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include "..\server\ilagcompensationmanager.h"
//-----------------------
// Fire Bullets? Prob left over from ASW!
//-----------------------
void CrHetoricalPlayer::FireBullets ( const FireBulletsInfo_t &info )
{
//	lagcompensation->StartLagCompensation( this, LAG_COMPENSATE_HITBOXES);

	BaseClass::FireBullets(info);

	lagcompensation->FinishLagCompensation( this );
}

extern int	gEvilImpulse101;
//-----------------------------------------------------------------------------
// Purpose: Player reacts to bumping a weapon. 
// Input  : pWeapon - the weapon that the player bumped into.
// Output : Returns true if player picked up the weapon
//-----------------------------------------------------------------------------
bool CrHetoricalPlayer::BumpWeapon( CBaseCombatWeapon *pWeapon )
{
	CBaseCombatCharacter *pOwner = pWeapon->GetOwner();

	// Can I have this weapon type?
	if ( !IsAllowedToPickupWeapons() )
		return false;

	if ( pOwner || !Weapon_CanUse( pWeapon ) || !g_pGameRules->CanHavePlayerItem( this, pWeapon ) )
	{
		if ( gEvilImpulse101 )
		{
			UTIL_Remove( pWeapon );
		}
		return false;
	}

	// Don't let the player fetch weapons through walls (use MASK_SOLID so that you can't pickup through windows)
	if( !pWeapon->FVisible( this, MASK_SOLID ) && !(GetFlags() & FL_NOTARGET) )
	{
		return false;
	}

	// ----------------------------------------
	// If I already have it just take the ammo
	// ----------------------------------------
	if (Weapon_OwnsThisType( pWeapon->GetClassname(), pWeapon->GetSubType())) 
	{
		//Only remove the weapon if we attained ammo from it
		if ( Weapon_EquipAmmoOnly( pWeapon ) == false )
			return false;

		// Only remove me if I have no ammo left
		// Can't just check HasAnyAmmo because if I don't use clips, I want to be removed, 
		if ( pWeapon->UsesClipsForAmmo1() && pWeapon->HasPrimaryAmmo() )
			return false;

		UTIL_Remove( pWeapon );
		return false;
	}
	// -------------------------
	// Otherwise take the weapon
	// -------------------------
	else 
	{
		//Make sure we're not trying to take a new weapon type we already have
		if ( Weapon_SlotOccupied( pWeapon ) )
		{
			CBaseCombatWeapon *pActiveWeapon = Weapon_GetSlot( WEAPON_PRIMARY_SLOT );

			if ( pActiveWeapon != NULL && pActiveWeapon->HasAnyAmmo() == false && Weapon_CanSwitchTo( pWeapon ) )
			{
				Weapon_Equip( pWeapon );
				return true;
			}

			//Attempt to take ammo if this is the gun we're holding already
			if ( Weapon_OwnsThisType( pWeapon->GetClassname(), pWeapon->GetSubType() ) )
			{
				Weapon_EquipAmmoOnly( pWeapon );
			}

			return false;
		}
	}

	pWeapon->CheckRespawn();
	Weapon_Equip( pWeapon );

	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------
// Walking
//-----------------------
void CrHetoricalPlayer::StartWalking( void )
{
	SetMaxSpeed( PLAYER_WALK_SPEED );
	m_fIsWalking = true;
}

void CrHetoricalPlayer::StopWalking( void )
{
	SetMaxSpeed( PLAYER_WALK_SPEED );
	m_fIsWalking = false;
}
void CrHetoricalPlayer::HandleSpeedChanges( void )
{
	bool bIsWalking = IsWalking();
	bool bWantWalking;
	
	bWantWalking = true;
	
	if( bIsWalking != bWantWalking )
	{
		if ( bWantWalking )
		{
			StartWalking();
		}
		else
		{
			StopWalking();
		}
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------
// Use + Pickup
//-----------------------
void CrHetoricalPlayer::PickupObject( CBaseEntity *pObject, bool bLimitMassAndSize )
{
	// can't pick up what you're standing on
	if ( GetGroundEntity() == pObject )
		return;

	if ( bLimitMassAndSize == true )
	{
		if ( CBasePlayer::CanPickupObject( pObject, PLAYER_MAX_LIFT_MASS, PLAYER_MAX_LIFT_SIZE ) == false )
			return;
	}

	// Can't be picked up if NPCs are on me
	if ( pObject->HasNPCsOnIt() )
		return;

	PlayerPickupObject( this, pObject );
}

void CrHetoricalPlayer::ForceDropOfCarriedPhysObjects( CBaseEntity *pOnlyIfHoldingThis )
{
	BaseClass::ForceDropOfCarriedPhysObjects( pOnlyIfHoldingThis );
}

void CrHetoricalPlayer::PlayerUse ( void )
{
	// Was use pressed or released?
	if ( ! ((m_nButtons | m_afButtonPressed | m_afButtonReleased) & IN_USE) )
		return;

	if ( m_afButtonPressed & IN_USE )
	{
		// Currently using a latched entity?
		if ( ClearUseEntity() )
		{
			return;
		}
		else
		{
			if ( m_afPhysicsFlags & PFLAG_DIROVERRIDE )
			{
				m_afPhysicsFlags &= ~PFLAG_DIROVERRIDE;
				m_iTrain = TRAIN_NEW|TRAIN_OFF;
				return;
			}
		}

		// Tracker 3926:  We can't +USE something if we're climbing a ladder
		if ( GetMoveType() == MOVETYPE_LADDER )
		{
			return;
		}
	}

	if( m_flTimeUseSuspended > gpGlobals->curtime )
	{
		// Something has temporarily stopped us being able to USE things.
		// Obviously, this should be used very carefully.(sjb)
		return;
	}

	CBaseEntity *pUseEntity = FindUseEntity();

	bool usedSomething = false;

	// Found an object
	if ( pUseEntity )
	{
		//!!!UNDONE: traceline here to prevent +USEing buttons through walls			
		int caps = pUseEntity->ObjectCaps();
		variant_t emptyVariant;

		if ( m_afButtonPressed & IN_USE )
		{
			// Robin: Don't play sounds for NPCs, because NPCs will allow respond with speech.
			if ( !pUseEntity->MyNPCPointer() )
			{
				EmitSound( "HL2Player.Use" );
				//EmitSound( "Player.Use" );
			}
		}

		if ( ( (m_nButtons & IN_USE) && (caps & FCAP_CONTINUOUS_USE) ) ||
			 ( (m_afButtonPressed & IN_USE) && (caps & (FCAP_IMPULSE_USE|FCAP_ONOFF_USE)) ) )
		{
			if ( caps & FCAP_CONTINUOUS_USE )
				m_afPhysicsFlags |= PFLAG_USING;

			pUseEntity->AcceptInput( "Use", this, this, emptyVariant, USE_TOGGLE );

			usedSomething = true;
		}
		// UNDONE: Send different USE codes for ON/OFF.  Cache last ONOFF_USE object to send 'off' if you turn away
		else if ( (m_afButtonReleased & IN_USE) && (pUseEntity->ObjectCaps() & FCAP_ONOFF_USE) )	// BUGBUG This is an "off" use
		{
			pUseEntity->AcceptInput( "Use", this, this, emptyVariant, USE_TOGGLE );

			usedSomething = true;
		}
	}

	else if ( m_afButtonPressed & IN_USE )
	{
		// Signal that we want to play the deny sound, unless the user is +USEing on a ladder!
		// The sound is emitted in ItemPostFrame, since that occurs after GameMovement::ProcessMove which
		// lets the ladder code unset this flag.
		m_bPlayUseDenySound = true;
	}

	// Debounce the use key
	if ( usedSomething && pUseEntity )
	{
		m_Local.m_nOldButtons |= IN_USE;
		m_afButtonPressed &= ~IN_USE;
	}
}

void CrHetoricalPlayer::PlayUseDenySound()
{
	m_bPlayUseDenySound = true;
}

void CrHetoricalPlayer::ItemPostFrame()
{
	BaseClass::ItemPostFrame();

	if ( m_bPlayUseDenySound )
	{
		m_bPlayUseDenySound = false;

		EmitSound( "HL2Player.UseDeny" );
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------
// Damage
//-----------------------
bool CrHetoricalPlayer::PassesDamageFilter( const CTakeDamageInfo &info )
{
	CBaseEntity *pAttacker = info.GetAttacker();
	if( pAttacker && pAttacker->MyNPCPointer())
	{
		return false;
	}

//	if( m_hPlayerProxy && !m_hPlayerProxy->PassesDamageFilter( info ) )
//	{
//		return false;
//	}

	return BaseClass::PassesDamageFilter( info );
}

int	CrHetoricalPlayer::OnTakeDamage( const CTakeDamageInfo &info )
{

	CTakeDamageInfo inputInfoCopy( info );

	// If you shoot yourself, make it hurt but push you less
	if ( inputInfoCopy.GetAttacker() == this && inputInfoCopy.GetDamageType() == DMG_BULLET )
	{
		inputInfoCopy.ScaleDamage( 5.0f );
		inputInfoCopy.ScaleDamageForce( 0.05f );
	}

	CBaseEntity *pAttacker = inputInfoCopy.GetAttacker();
	CBaseEntity *pInflictor = inputInfoCopy.GetInflictor();

	// ignore fall damage if instructed to do so by input
	if ( ( info.GetDamageType() & DMG_FALL ) )
	{
		inputInfoCopy.SetDamage(0.0f);
	}

	// Refuse damage from any Physics.
	if ( (pAttacker && FClassnameIs( pAttacker, "prop_physics" )) ||
		(pInflictor && FClassnameIs( pInflictor, "prop_physics" ))  )
	{
		inputInfoCopy.SetDamage(0.0f);
	}

	// Refuse damage from our ents.
	if ( (pAttacker && FClassnameIs( pAttacker, "prop_puzzle_box" )) ||
		(pInflictor && FClassnameIs( pInflictor, "prop_puzzle_box" ))  )
	{
		inputInfoCopy.SetDamage(0.0f);
	}

	int ret = BaseClass::OnTakeDamage( inputInfoCopy );
	m_DmgOrigin = info.GetDamagePosition();

	if ( GetHealth() < 100 )
	{
		m_fTimeLastHurt = gpGlobals->curtime;
	}

	return ret;
}

int CrHetoricalPlayer::OnTakeDamage_Alive( const CTakeDamageInfo &info )
{
	// set damage type sustained
	m_bitsDamageType |= info.GetDamageType();

	if ( !CBaseCombatCharacter::OnTakeDamage_Alive( info ) )
		return 0;

	CBaseEntity * attacker = info.GetAttacker();

	if ( !attacker )
		return 0;

	Vector vecDir = vec3_origin;
	if ( info.GetInflictor() )
	{
		vecDir = info.GetInflictor()->WorldSpaceCenter() - Vector ( 0, 0, 10 ) - WorldSpaceCenter();
		VectorNormalize( vecDir );
	}

	if ( info.GetInflictor() && (GetMoveType() == MOVETYPE_WALK) && 
		( !attacker->IsSolidFlagSet(FSOLID_TRIGGER)) )
	{
		Vector force = vecDir;// * -DamageForce( WorldAlignSize(), info.GetBaseDamage() );
		if ( force.z > 250.0f )
		{
			force.z = 250.0f;
		}
		ApplyAbsVelocityImpulse( force );
	}

	// fire global game event

	IGameEvent * event = gameeventmanager->CreateEvent( "player_hurt" );
	if ( event )
	{
		event->SetInt("userid", GetUserID() );
		event->SetInt("health", max(0, m_iHealth) );
		event->SetInt("priority", 5 );	// HLTV event priority, not transmitted

		if ( attacker->IsPlayer() )
		{
			CBasePlayer *player = ToBasePlayer( attacker );
			event->SetInt("attacker", player->GetUserID() ); // hurt by other player
		}
		else
		{
			event->SetInt("attacker", 0 ); // hurt by "world"
		}

		gameeventmanager->FireEvent( event );
	}

	// Insert a combat sound so that nearby NPCs hear battle
	if ( attacker->IsNPC() )
	{
		CSoundEnt::InsertSound( SOUND_COMBAT, GetAbsOrigin(), 512, 0.5, this );
	}

	return 1;
}

void CrHetoricalPlayer::OnDamagedByExplosion( const CTakeDamageInfo &info )
{
	if ( info.GetInflictor() && info.GetInflictor()->ClassMatches( "mortarshell" ) )
	{
		// No ear ringing for mortar
		UTIL_ScreenShake( info.GetInflictor()->GetAbsOrigin(), 4.0, 1.0, 0.5, 1000, SHAKE_START, false );
		return;
	}
	BaseClass::OnDamagedByExplosion( info );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------
// Impulse 101
//-----------------------
extern int	gEvilImpulse101;

void CrHetoricalPlayer::CheatImpulseCommands( int iImpulse )
{
	switch ( iImpulse )
	{
	case 101:
		{
			if( sv_cheats->GetBool() )
			{
				GiveAllItems();
			}
		}
		break;

	default:
		BaseClass::CheatImpulseCommands( iImpulse );
	}
}

void CrHetoricalPlayer::GiveAllItems( void )
{
	GiveNamedItem( "weapon_puntgun" );
}

void CrHetoricalPlayer::GiveDefaultItems( void )
{
	castable_string_t st( "suit_no_sprint" );
	GlobalEntity_SetState( st, GLOBAL_OFF );
	InputDisableFlashlight( inputdata_t() );
}
