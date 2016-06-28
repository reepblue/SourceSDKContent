#include "cbase.h"
#include "predicted_viewmodel.h"
#include "ai_speech.h"			// For expresser host
#include "hintmessage.h"
#include "sdk_shareddefs.h"

//-----NonOptional Defines-----
// Pretty much values here can be changed.
// Should be self explanitory.
//-----------------------------

// How fast can the player walk?
#define PLAYER_WALK_SPEED player_walkspeed.GetFloat()

// Or pickup limits.
#define PLAYER_MAX_LIFT_MASS 85
#define PLAYER_MAX_LIFT_SIZE 128

// Sounds.
#define SOUND_HINT		"Hint.Display"
#define SOUND_USE_DENY	"Player.UseDeny"
#define SOUND_USE		"Player.Use"

// Definitions for weapon slots
#define	WEAPON_MELEE_SLOT			0
#define	WEAPON_SECONDARY_SLOT		1
#define	WEAPON_PRIMARY_SLOT			2
#define	WEAPON_EXPLOSIVE_SLOT		3
#define	WEAPON_TOOL_SLOT			4

//-----------------------------------------------------------------------------
// Purpose: Used to relay outputs/inputs from the player to the world and viceversa
//-----------------------------------------------------------------------------
class CLogicPlayerProxy : public CLogicalEntity
{
	DECLARE_CLASS( CLogicPlayerProxy, CLogicalEntity );

private:

	DECLARE_DATADESC();

public:

	COutputEvent m_OnFlashlightOn;
	COutputEvent m_OnFlashlightOff;
	COutputEvent m_PlayerHasAmmo;
	COutputEvent m_PlayerHasNoAmmo;
	COutputEvent m_PlayerDied;
	COutputEvent m_PlayerMissedAR2AltFire; // Player fired a combine ball which did not dissolve any enemies. 

	COutputInt m_RequestedPlayerHealth;

	void InputRequestPlayerHealth( inputdata_t &inputdata );
	void InputSetFlashlightSlowDrain( inputdata_t &inputdata );
	void InputSetFlashlightNormalDrain( inputdata_t &inputdata );
	void InputSetPlayerHealth( inputdata_t &inputdata );
	void InputRequestAmmoState( inputdata_t &inputdata );
	void InputLowerWeapon( inputdata_t &inputdata );
	void InputEnableCappedPhysicsDamage( inputdata_t &inputdata );
	void InputDisableCappedPhysicsDamage( inputdata_t &inputdata );
	void InputSetLocatorTargetEntity( inputdata_t &inputdata );

	void Activate ( void );

	bool PassesDamageFilter( const CTakeDamageInfo &info );

	EHANDLE m_hPlayer;
};

class CSDKPlayer : public CBasePlayer
{
public:
	DECLARE_CLASS( CSDKPlayer, CBasePlayer );
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	// Basics
	CSDKPlayer();
	~CSDKPlayer();

	static CSDKPlayer *CreatePlayer( const char *className, edict_t *ed )
	{
		CSDKPlayer::s_PlayerEdict = ed;
		return (CSDKPlayer*)CreateEntityByName( className );
	}

	virtual void Precache();
	virtual void Spawn();
	virtual void UpdateClientData( void );
	virtual void PostThink();
	virtual void PreThink();
	virtual void Splash( void );
	CLogicPlayerProxy	*GetPlayerProxy( void );

#ifndef SWARM_DLL
	void FirePlayerProxyOutput( const char *pszOutputName, variant_t variant, CBaseEntity *pActivator, CBaseEntity *pCaller );
	EHANDLE			m_hPlayerProxy;	// Handle to a player proxy entity for quicker reference
#endif

	// Use + Pickup
	virtual void PlayerUse( void );
	virtual void PickupObject( CBaseEntity *pObject, bool bLimitMassAndSize );
	virtual void ForceDropOfCarriedPhysObjects( CBaseEntity *pOnlyIfHoldingThis );
	virtual void ClearUsePickup();
	virtual bool CanPickupObject( CBaseEntity *pObject, float massLimit, float sizeLimit );
	CNetworkVar( bool, m_bPlayerPickedUpObject );
	bool PlayerHasObject() { return m_bPlayerPickedUpObject; }

	// This float is outside PLAYER_MOUSEOVER_HINTS to make things much easier. 
	float m_flNextMouseoverUpdate;

	// Viewmodel + Weapon
	virtual void CreateViewModel( int index );
	virtual void Weapon_Equip ( CBaseCombatWeapon *pWeapon );
	virtual bool Weapon_Switch( CBaseCombatWeapon *pWeapon, int viewmodelindex = 0);
	virtual bool BumpWeapon( CBaseCombatWeapon *pWeapon );
	CNetworkVar( int, m_iShotsFired );	// number of shots fired recently

	//Walking
	virtual void StartWalking( void );
	virtual void StopWalking( void );
	CNetworkVarForDerived( bool, m_fIsWalking );
	virtual bool IsWalking( void ) { return m_fIsWalking; }
	virtual void  HandleSpeedChanges( void );
	bool  m_bPlayUseDenySound;		// Signaled by PlayerUse, but can be unset by HL2 ladder code...

	// Damage
	virtual bool PassesDamageFilter( const CTakeDamageInfo &info );
	virtual int OnTakeDamage( const CTakeDamageInfo &info );
	virtual int OnTakeDamage_Alive( const CTakeDamageInfo &info );
	virtual void OnDamagedByExplosion( const CTakeDamageInfo &info );
	virtual void Event_Killed( const CTakeDamageInfo &info );

#ifdef PLAYER_HEALTH_REGEN
	//Regenerate
	float m_fTimeLastHurt;
	bool  m_bIsRegenerating;		// Is the player currently regaining health
#endif

	// Lag compensate when firing bullets
	//void FireBullets ( const FireBulletsInfo_t &info );

	// CHEAT
	virtual void CheatImpulseCommands( int iImpulse );
	virtual void GiveAllItems( void );
	virtual void GiveDefaultItems( void );

	// Hints
	virtual void HintMessage( const char *pMessage, bool bDisplayIfDead, bool bOverrideClientSettings = false, bool bQuiet = false ); // Displays a hint message to the player
	CHintMessageQueue *m_pHintMessageQueue;
	unsigned int m_iDisplayHistoryBits;
	bool m_bShowHints;

	// Hint Flags: These are flags to tick when the hint shows. This prevents players feeling that the game think's they are stupid by
	// repeating the same hint over, and over.
	#define DHF_GAME_STARTED		( 1 << 1 )
	#define DHF_GAME_ENDED			( 1 << 2 )
	// etc....

	// You can as many flags as you wish.

#ifdef PLAYER_MOUSEOVER_HINTS
	void UpdateMouseoverHints();
#endif

	Class_T Classify ( void );

public:
	void FireBullet( 
		Vector vecSrc, 
		const QAngle &shootAngles, 
		float vecSpread, 
		int iDamage, 
		int iBulletType,
		CBaseEntity *pevAttacker,
		bool bDoEffects,
		float x,
		float y );
private:
		float				m_flTimeUseSuspended;

		Vector				m_vecMissPositions[16];
		int					m_nNumMissPositions;

protected:
		virtual void		ItemPostFrame();
		virtual void		PlayUseDenySound();
};

inline CSDKPlayer *To_SDKPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

#ifdef _DEBUG
	Assert( dynamic_cast<CSDKPlayer*>( pEntity ) != 0 );
#endif
	return static_cast< CSDKPlayer* >( pEntity );
}