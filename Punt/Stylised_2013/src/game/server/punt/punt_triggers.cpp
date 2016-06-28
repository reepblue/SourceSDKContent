//=========== Copyright © 2012, rHetorical, All rights reserved. =============
//
// Purpose: Punt's box.
//		
//=============================================================================

#include "cbase.h"
#include "weapon_physcannon.h"
#include "hl2_player.h"
#include <convar.h>
#include "saverestore_utlvector.h"
#include "triggers.h"

ConVar filter_toggle( "filter_toggle", "0", FCVAR_HIDDEN );

//-----------------------------------------------------------------------------
// Cleanser
//-----------------------------------------------------------------------------
class CTriggerCleanser : public CTriggerMultiple
{
	DECLARE_CLASS( CTriggerCleanser, CTriggerMultiple );
	DECLARE_DATADESC();

public:

	//Constructor
	CTriggerCleanser()
	{
		m_bSoundon = false;
		m_bEnableSound = false;

	}

	void Precache( void );
	void Spawn( void );
	void Think( void );
	void ThinkSound( void );
	void Touch( CBaseEntity *pOther );
	void EndTouch( CBaseEntity *pOther );


private:
	void InputEnable( inputdata_t &inputdata );
	void InputDisable( inputdata_t &inputdata );
	void InputToggle( inputdata_t &inputdata );

	int m_nDissolveType;
	int m_nFilterType;
	bool m_bSoundon;
	bool m_bEnableSound;

	COutputEvent m_OnDissolveBox;
};


//-----------------------------------------------------------------------------
// Save/load
//-----------------------------------------------------------------------------
LINK_ENTITY_TO_CLASS( trigger_cleanser, CTriggerCleanser );

BEGIN_DATADESC( CTriggerCleanser )

	DEFINE_KEYFIELD( m_nDissolveType,	FIELD_INTEGER,	"dissolvetype" ),
	DEFINE_KEYFIELD( m_nFilterType,	FIELD_INTEGER,	"filtertype" ),
	DEFINE_OUTPUT( m_OnDissolveBox, "OnDissolveBox" ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Toggle", InputToggle ),
	DEFINE_KEYFIELD( m_bEnableSound, FIELD_BOOLEAN, "Hum" ),
	DEFINE_THINKFUNC( ThinkSound ),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTriggerCleanser::Precache( void )
{
	PrecacheScriptSound( "Punt.Fizzle" ); 
	PrecacheScriptSound( "Punt.Filterloop" );

	BaseClass::Precache();
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTriggerCleanser::Think( void )
{
	if ( !m_bDisabled )
	{
		RemoveEffects( EF_NODRAW );
		if ( m_nFilterType == 1 )
		{
			SetSolid( SOLID_BSP );
		}
	}
	else
	{
		AddEffects( EF_NODRAW );
		if ( m_nFilterType == 1 )
		{
			SetSolid( SOLID_NONE );
		}
	}

	SetNextThink( gpGlobals->curtime );
	BaseClass::Think();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTriggerCleanser::ThinkSound( void )
{
	if ( m_bEnableSound && !m_bSoundon )
	{
		EmitSound( "Punt.Filterloop" );
		m_bSoundon = true;
	}
	SetNextThink(gpGlobals->curtime + .1, "Sound" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CTriggerCleanser::Spawn( void )
{
	SetNextThink( gpGlobals->curtime );
	m_bSoundon = false;
	if (m_bEnableSound )
	{
		RegisterThinkContext( "Sound" );
		SetContextThink( &CTriggerCleanser::ThinkSound, gpGlobals->curtime, "Sound" );
	}

	if ( m_nFilterType == 1 )
	{
		SetSolid( SOLID_BSP );
	}
	BaseClass::Spawn();
}

//------------------------------------------------------------------------------
// Inputs
//------------------------------------------------------------------------------
void CTriggerCleanser::InputToggle( inputdata_t &inputdata )
{
	if ( m_bDisabled )
	{
		InputEnable( inputdata );
	}
	else
	{
		InputDisable( inputdata );
	}
}

void CTriggerCleanser::InputEnable( inputdata_t &inputdata )
{
	if ( m_bDisabled )
	{
		RemoveEffects( EF_NODRAW );
		m_bSoundon = false;
		Enable();
	}
}

void CTriggerCleanser::InputDisable( inputdata_t &inputdata )
{
	if ( !m_bDisabled )
	{
		StopSound( "Punt.Filterloop" );
		EmitSound( "Punt.FilterActivate" );
		AddEffects( EF_NODRAW );
		Disable();
	}
}

//-----------------------------------------------------------------------------
// Traps the entities
//-----------------------------------------------------------------------------
void CTriggerCleanser::Touch( CBaseEntity *pOther )
{
	if ( !PassesTriggerFilters(pOther) )
		return;

	CBaseAnimating *pAnim = pOther->GetBaseAnimating();
	if ( !pAnim )
		return;
	if ( pOther->IsPlayer())
	{
		if ( m_nFilterType == 0 )
		{
			filter_toggle.SetValue(1);
		}
		if ( m_nFilterType == 2 )
		{
			CTakeDamageInfo info( this, this, 1000, DMG_BURN );
			pOther->TakeDamage( info );
		}
	}
	else
	{	
		if ( m_nFilterType == 0 )
		{
			pAnim->Dissolve( NULL, gpGlobals->curtime, false, m_nDissolveType );
			if ( pOther->ClassMatches("prop_punt_box") )
			{
				m_OnDissolveBox.FireOutput( this, this );
			}
		}
	}

}
void CTriggerCleanser::EndTouch( CBaseEntity *pOther )
{
	CBaseAnimating *pAnim = pOther->GetBaseAnimating();
	if ( !pAnim )
		return;
	if ( pOther->IsPlayer())
	{
		ConVar *filter_toggle = cvar->FindVar( "filter_toggle" );
		filter_toggle->SetValue(0);
	}
}

//-----------------------------------------------------------------------------
// nopunt volume NOTE: THIS IS BROKE DUE TO CLEANSERS!
//-----------------------------------------------------------------------------
class CFuncNoPuntVol : public CTriggerMultiple
{
	DECLARE_CLASS( CFuncNoPuntVol, CTriggerMultiple );
	DECLARE_DATADESC();

public:
	void Precache( void );
	void Spawn( void );
	void Touch( CBaseEntity *pOther );
	void EndTouch( CBaseEntity *pOther );


private:
	void InputEnable( inputdata_t &inputdata );
	void InputDisable( inputdata_t &inputdata );
	void InputToggle( inputdata_t &inputdata );
};


//-----------------------------------------------------------------------------
// Save/load
//-----------------------------------------------------------------------------
LINK_ENTITY_TO_CLASS( func_nopunt_volume, CFuncNoPuntVol );

BEGIN_DATADESC( CFuncNoPuntVol )

	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Toggle", InputToggle ),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFuncNoPuntVol::Precache( void )
{
	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFuncNoPuntVol::Spawn( void )
{
	BaseClass::Spawn();
}

//------------------------------------------------------------------------------
// Inputs
//------------------------------------------------------------------------------
void CFuncNoPuntVol::InputToggle( inputdata_t &inputdata )
{
	if ( m_bDisabled )
	{
		InputEnable( inputdata );
	}
	else
	{
		InputDisable( inputdata );
	}
}

void CFuncNoPuntVol::InputEnable( inputdata_t &inputdata )
{
	if ( m_bDisabled )
	{
		Enable();
	}
}

void CFuncNoPuntVol::InputDisable( inputdata_t &inputdata )
{
	if ( !m_bDisabled )
	{
		Disable();
	}
}

//-----------------------------------------------------------------------------
// Traps the entities
//-----------------------------------------------------------------------------
void CFuncNoPuntVol::Touch( CBaseEntity *pEntity )
{
	if ( !PassesTriggerFilters(pEntity) )
		return;

	CBaseAnimating *pAnim = pEntity->GetBaseAnimating();
	if ( !pAnim )
		return;

	if ( pEntity->ClassMatches("prop_punt_box") )
	{
		pEntity->SetCollisionGroup( COLLISION_GROUP_UNPUNTABLE );
	}
}

void CFuncNoPuntVol::EndTouch( CBaseEntity *pOther )
{
	CBaseAnimating *pAnim = pOther->GetBaseAnimating();
	if ( !pAnim )
		return;
	if ( pOther->ClassMatches("prop_punt_box") )
	{
		pOther->SetCollisionGroup( COLLISION_GROUP_PUNTABLE );
	}
}
