//=========== Copyright © 2013, rHetorical, All rights reserved. =============
//
// Purpose: A Player that is on top of HL2_player. Replace "rHetotical" with
// The current name. (e.g: CrHetoricalPlayer should be CPuntPlayer for PUNT.)
//		
//=============================================================================

#include "cbase.h"
#include "predicted_viewmodel.h"
#include "hl2_playerlocaldata.h"
#include "hl2_player.h"
#include "ai_speech.h"			// For expresser host

class CrHetoricalPlayer : public CAI_ExpresserHost<CHL2_Player>  //public CBasePlayer
{
public:
	DECLARE_CLASS( CrHetoricalPlayer, CHL2_Player );
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	// Basics
	virtual void Precache();
	virtual void Spawn();
	void PostThink();

	// Use + Pickup
	void PlayerUse( void );
	virtual void PickupObject( CBaseEntity *pObject, bool bLimitMassAndSize );
	virtual void ForceDropOfCarriedPhysObjects( CBaseEntity *pOnlyIfHoldingThis );

	// Viewmodel + Weapon
	void CreateViewModel( int index );
	virtual bool Weapon_Switch( CBaseCombatWeapon *pWeapon, int viewmodelindex = 0);
	virtual bool BumpWeapon( CBaseCombatWeapon *pWeapon );

	//Walking
	void StartWalking( void );
	void StopWalking( void );
	CNetworkVarForDerived( bool, m_fIsWalking );
	bool IsWalking( void ) { return m_fIsWalking; }
	void  HandleSpeedChanges( void );
	bool  m_bPlayUseDenySound;		// Signaled by PlayerUse, but can be unset by HL2 ladder code...

	// Damage
	bool PassesDamageFilter( const CTakeDamageInfo &info );
	int OnTakeDamage( const CTakeDamageInfo &info );
	int OnTakeDamage_Alive( const CTakeDamageInfo &info );
	void OnDamagedByExplosion( const CTakeDamageInfo &info );

	//Regenerate
	float m_fTimeLastHurt;
	bool  m_bIsRegenerating;		// Is the player currently regaining health

	// Lag compensate when firing bullets
	void FireBullets ( const FireBulletsInfo_t &info );

	// CHEAT
	void CheatImpulseCommands( int iImpulse );
	void GiveAllItems( void );
	void GiveDefaultItems( void );

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

protected:
		virtual void		ItemPostFrame();
		virtual void		PlayUseDenySound();
};

inline CrHetoricalPlayer *To_rHetoricalPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsPlayer() )
		return NULL;

#ifdef _DEBUG
	Assert( dynamic_cast<CrHetoricalPlayer*>( pEntity ) != 0 );
#endif
	return static_cast< CrHetoricalPlayer* >( pEntity );
}

