//=========== Copyright © 2013, rHetorical, All rights reserved. =============
//
// Purpose: 
//		
//=============================================================================

#ifndef PROP_PUZZLE_BOX_H
#define PROP_PUZZLE_BOX_H

#ifdef _WIN32
#pragma once
#endif

#include "cbase.h"
#include "player_pickup.h"

#define BOX_MODEL "models/props_gameplay/physcube.mdl"

////
//-----------------------------------------------------------------------------
// Purpose: Base
//-----------------------------------------------------------------------------
class CPuzzleBox : public CBaseAnimating, public CDefaultPlayerPickupVPhysics
{
	public:
	DECLARE_CLASS( CPuzzleBox, CBaseAnimating );
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();

	bool CreateVPhysics()
	{
		VPhysicsInitNormal( SOLID_VPHYSICS, 0, false );
		return true;
	}

	CNetworkVar( float, m_flScaleX );
	CNetworkVar( float, m_flScaleY );
	CNetworkVar( float, m_flScaleZ );

	CNetworkVar( float, m_flLerpTimeX );
	CNetworkVar( float, m_flLerpTimeY );
	CNetworkVar( float, m_flLerpTimeZ );

	CNetworkVar( float, m_flGoalTimeX );
	CNetworkVar( float, m_flGoalTimeY );
	CNetworkVar( float, m_flGoalTimeZ );

	void Precache( void );
	void Spawn( void );
	void UseRandomColor( bool state = false ) { m_bUseRandomColor = state; }
	void SetTemplated( bool state = false ) {  m_bSetTemplated = state; }
	void FadeIn( bool state = false ) {  m_bStartFadeIn = state; }
	void OnDissolve();
	void RandomColor();
	void Shrink();
	void ShrinkKill();
	void ThinkFadeIn();
	void MakePlanez();

	void BecomePlane();

	// Use
	void OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason );
	void OnPhysGunDrop( CBasePlayer *pPhysGunUser, PhysGunDrop_t reason );
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	int ObjectCaps();
	float m_flFinalSize;
	bool m_bIsPlane;
	bool m_bMadePlane;

	void InputDissolve( inputdata_t &inputData );
	void InputSetColor( inputdata_t &inputData );
	void InputRandomColor( inputdata_t &inputData );

private:

	bool m_bSetTemplated;
	bool m_bUseRandomColor;
	bool m_bShrinking;
	bool m_bStartFadeIn;
	bool m_bRightColor;

	CHandle<CBasePlayer>	m_hPhysicsAttacker;
	color32	m_clrBox;
	Vector MdlTop;

	// Pickup
	COutputEvent m_OnPlayerUse;
	COutputEvent m_OnPlayerPickup;
	COutputEvent m_OnPhysGunPickup;
	COutputEvent m_OnPhysGunDrop;

	// Death!
	COutputEvent m_OnDissolved;

};
///
#endif // PROP_PUZZLE_BOX_H
