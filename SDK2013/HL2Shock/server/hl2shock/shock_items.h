//=========== Copyright © 2015, rHetorical, All rights reserved. =============
//
// Purpose: 
//		
//=============================================================================

#ifndef SHOCK_ITEMS_H
#define SHOCK_ITEMS_H

#ifdef _WIN32
#pragma once
#endif

#include "entityoutput.h"
#include "player_pickup.h"
#include "vphysics/constraints.h"

class CShockItem : public CBaseAnimating
{
public:
	DECLARE_CLASS(CShockItem, CBaseAnimating);

	DECLARE_DATADESC();
	CShockItem(){};

	virtual void Spawn(void);
	virtual void Precache();

	bool CreateVPhysics()
	{
		VPhysicsInitNormal(SOLID_VPHYSICS, 0, false);
		return true;
	}

	//Use
	void Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	int ObjectCaps();
	virtual bool GiveItem(CBasePlayer *pPlayer) { return false; };

	// Become useable when we are at rest
	virtual void OnEntityEvent(EntityEvent_t event, void *pEventData);

	// Activate when at rest, but don't allow pickup until then
	void ActivateWhenAtRest(float flTime = 0.5f);

private:

	bool m_bActivateWhenAtRest;
	COutputEvent m_OnPlayerTouch;
	COutputEvent m_OnCacheInteraction;

protected:
	virtual void ComeToRest(void);

};

#endif // SHOCK_ITEMS_H
