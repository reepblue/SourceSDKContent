//=========== Copyright © 2015, rHetorical, All rights reserved. =============
//
// Purpose: 
//		
//=============================================================================

#include "cbase.h"
#include "shock_items.h"
#include "hl2_player.h"
#include "player.h"
#include "engine/ienginesound.h"
#include "soundent.h"
#include "player_pickup.h"
#include "soundenvelope.h"
#include "game.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS(item_shock_base, CShockItem);

BEGIN_DATADESC(CShockItem)

	//Save/load
	DEFINE_USEFUNC(Use),
	DEFINE_THINKFUNC(ComeToRest),

	// Outputs
	DEFINE_OUTPUT(m_OnPlayerTouch, "OnPlayerTouch"),
	DEFINE_OUTPUT(m_OnCacheInteraction, "OnCacheInteraction"),
END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CShockItem::Precache(void)
{
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: Sets up the entity's initial state
//-----------------------------------------------------------------------------
void CShockItem::Spawn(void)
{
	Precache();

	if (g_pGameRules->IsAllowedToSpawn(this) == false)
	{
		UTIL_Remove(this);
		return;
	}

	CreateVPhysics();
	//SetMoveType(MOVETYPE_FLYGRAVITY);
	SetSolid(SOLID_VPHYSICS);
	SetBlocksLOS(false);
	AddEFlags(EFL_NO_ROTORWASH_PUSH);

	AddEffects(EF_ITEM_BLINK);
	AddSolidFlags(FSOLID_USE_TRIGGER_BOUNDS);
	SetCollisionGroup(COLLISION_GROUP_INTERACTIVE_DEBRIS);

	m_takedamage = DAMAGE_EVENTS_ONLY;

	BaseClass::Spawn();

}

//-----------------------------------------------------------------------------
// Here we link +USE to the entity
//-----------------------------------------------------------------------------
int CShockItem::ObjectCaps()
{
	int caps = BaseClass::ObjectCaps();

	caps |= FCAP_IMPULSE_USE;

	return caps;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CShockItem::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	CBasePlayer *pPlayer = ToBasePlayer(pActivator);

	if (pPlayer == NULL)
		return;

	if (m_bActivateWhenAtRest)
		return;

	m_OnCacheInteraction.FireOutput(pActivator, this);

	if (GiveItem(pPlayer))
	{
		m_OnPlayerTouch.FireOutput(pActivator, this);
		UTIL_Remove(this);
	}
}

//-----------------------------------------------------------------------------
// Activate when at rest, but don't allow pickup until then
//-----------------------------------------------------------------------------
void CShockItem::ActivateWhenAtRest(float flTime /* = 0.5f */)
{
	RemoveSolidFlags(FSOLID_USE_TRIGGER_BOUNDS);
	m_bActivateWhenAtRest = true;
	SetThink(&CShockItem::ComeToRest);
	SetNextThink(gpGlobals->curtime + flTime);
}

//-----------------------------------------------------------------------------
// Become touchable when we are at rest
//-----------------------------------------------------------------------------
void CShockItem::OnEntityEvent(EntityEvent_t event, void *pEventData)
{
	BaseClass::OnEntityEvent(event, pEventData);

	switch (event)
	{
	case ENTITY_EVENT_WATER_TOUCH:
	{
		// Delay rest for a sec, to avoid changing collision 
		// properties inside a collision callback.
		SetThink(&CShockItem::ComeToRest);
		SetNextThink(gpGlobals->curtime + 0.1f);
	}
	break;
	}
}

//-----------------------------------------------------------------------------
// Become touchable when we are at rest
//-----------------------------------------------------------------------------
void CShockItem::ComeToRest(void)
{
	if (m_bActivateWhenAtRest)
	{
		m_bActivateWhenAtRest = false;
		AddSolidFlags(FSOLID_USE_TRIGGER_BOUNDS);
		SetThink(NULL);
	}
}

class CMedHypo : public CShockItem
{
public:
	DECLARE_CLASS(CMedHypo, CShockItem);

	void Spawn(void)
	{
		Precache();
		SetModel("models/items/medhypo.mdl");
		BaseClass::Spawn();
	}
	void Precache(void)
	{
		PrecacheModel("models/items/medhypo.mdl");
	}
	bool GiveItem(CBasePlayer *pPlayer)
	{
		CHL2_Player *HL2Player = ToHL2Player(pPlayer);
		if (HL2Player->GetMedHypoAmmount() >= HYPO_CARRY_LIMIT)
		{
			return false; 
		}

		CSingleUserRecipientFilter user(HL2Player);
		user.MakeReliable();

		UserMessageBegin(user, "ItemPickup");
		WRITE_STRING(GetClassname());
		MessageEnd();

		CPASAttenuationFilter filter(HL2Player, "Player.PickupWeapon");
		EmitSound(filter, HL2Player->entindex(), "Player.PickupWeapon");

		HL2Player->GiveMedHypo();
		return true;
	}
};

LINK_ENTITY_TO_CLASS(item_medhypo, CMedHypo);