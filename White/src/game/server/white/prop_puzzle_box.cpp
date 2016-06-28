//=========== Copyright © 2013, rHetorical, All rights reserved. =============
//
// Purpose: 
//		
//=============================================================================

#include "cbase.h"
#include "engine/IEngineSound.h"
#include "prop_puzzle_box.h"
#include "white_player.h" //Your Player file
#include "filters.h"
#include "physics.h"
#include "vphysics_interface.h"
#include "entityoutput.h"
#include "studio.h"
#include "explode.h"
#include <convar.h>
#include "particle_parse.h"
#include "props.h"
#include "vectronic/weapon_physcannon.h"
#include "physics_collisionevent.h"
#include "player_pickup.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

CON_COMMAND(ent_create_puzzle_box, "Creates an instance of the box in front of the player.")
{
	Vector vecForward;
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if(!pPlayer)
	{
		Warning("Could not determine calling player!\n");
		return;
	}
 
	AngleVectors( pPlayer->EyeAngles(), &vecForward );
	CPuzzleBox *pEnt = static_cast<CPuzzleBox*>( CreateEntityByName( "prop_puzzle_box" ) );
	if ( pEnt )
	{
		Vector vecOrigin = pPlayer->GetAbsOrigin() + vecForward * 64 + Vector(0,0,64);
		QAngle vecAngles(0, pPlayer->GetAbsAngles().y - 90, 0);
		pEnt->SetAbsOrigin(vecOrigin);
		pEnt->SetAbsAngles(vecAngles);
		pEnt->UseRandomColor( true );
		pEnt->Spawn();
	}
}

LINK_ENTITY_TO_CLASS( prop_puzzle_box, CPuzzleBox );
PRECACHE_REGISTER(prop_puzzle_box);

BEGIN_DATADESC( CPuzzleBox )

	//Save/load
	DEFINE_USEFUNC( Use ),

	// I/O
	DEFINE_FIELD( m_clrBox, FIELD_COLOR32 ),
	DEFINE_FIELD( m_bSetTemplated, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bMadePlane, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bShrinking, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flFinalSize, FIELD_FLOAT ),
	DEFINE_KEYFIELD( m_clrBox, FIELD_COLOR32, "boxcolor" ),
	DEFINE_KEYFIELD( m_bUseRandomColor, FIELD_BOOLEAN, "randomcolor" ),
	DEFINE_KEYFIELD(m_bStartFadeIn, FIELD_BOOLEAN, "fadein" ),
	DEFINE_THINKFUNC( ShrinkKill ),
	DEFINE_THINKFUNC( ThinkFadeIn ),

	DEFINE_INPUTFUNC( FIELD_VOID, "Dissolve", InputDissolve ),
	DEFINE_INPUTFUNC( FIELD_VOID, "SetColor", InputSetColor ),
	DEFINE_INPUTFUNC( FIELD_VOID, "RandomColor", InputRandomColor ),

	// I/O Pickup
	DEFINE_OUTPUT( m_OnDissolved, "OnDissolve" ),
	DEFINE_OUTPUT( m_OnPlayerUse, "OnPlayerUse" ),
	DEFINE_OUTPUT( m_OnPlayerPickup, "OnPlayerPickup" ),
	DEFINE_OUTPUT( m_OnPhysGunPickup, "OnPhysGunPickup" ),
	DEFINE_OUTPUT( m_OnPhysGunDrop, "OnPhysGunDrop" ),

	DEFINE_FIELD( m_flScaleX, FIELD_FLOAT ),
	DEFINE_FIELD( m_flScaleY, FIELD_FLOAT ),
	DEFINE_FIELD( m_flScaleZ, FIELD_FLOAT ),

	DEFINE_FIELD( m_flLerpTimeX, FIELD_FLOAT ),
	DEFINE_FIELD( m_flLerpTimeY, FIELD_FLOAT ),
	DEFINE_FIELD( m_flLerpTimeZ, FIELD_FLOAT ),

	DEFINE_FIELD( m_flGoalTimeX, FIELD_FLOAT ),
	DEFINE_FIELD( m_flGoalTimeY, FIELD_FLOAT ),
	DEFINE_FIELD( m_flGoalTimeZ, FIELD_FLOAT ),

END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CPuzzleBox, DT_PropPuzzleBox )
	SendPropFloat( SENDINFO(m_flScaleX), 0, SPROP_NOSCALE ),
	SendPropFloat( SENDINFO(m_flScaleY), 0, SPROP_NOSCALE ),
	SendPropFloat( SENDINFO(m_flScaleZ), 0, SPROP_NOSCALE ),

	SendPropFloat( SENDINFO(m_flLerpTimeX), 0, SPROP_NOSCALE ),
	SendPropFloat( SENDINFO(m_flLerpTimeY), 0, SPROP_NOSCALE ),
	SendPropFloat( SENDINFO(m_flLerpTimeZ), 0, SPROP_NOSCALE ),

	SendPropFloat( SENDINFO(m_flGoalTimeX), 0, SPROP_NOSCALE ),
	SendPropFloat( SENDINFO(m_flGoalTimeY), 0, SPROP_NOSCALE ),
	SendPropFloat( SENDINFO(m_flGoalTimeZ), 0, SPROP_NOSCALE ),
END_SEND_TABLE()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPuzzleBox::Precache( void )
{
	PrecacheModel( BOX_MODEL );
	PrecacheModel ("models/props_shapes/plane.mdl");

	BaseClass::Precache();
}

void CPuzzleBox::Spawn( void )
{
	Precache();
	SetModel( BOX_MODEL );
	SetSolid( SOLID_VPHYSICS );
	PrecacheParticleSystem( "puzzlecube_death" );

	SetUse( &CPuzzleBox::Use );
	CreateVPhysics();

	if( m_bStartFadeIn )
	{
		m_flScaleX = m_flFinalSize;
		m_flScaleY = m_flFinalSize;
		m_flScaleZ = m_flFinalSize;
		SetThink( &CPuzzleBox::ThinkFadeIn );
		SetNextThink(gpGlobals->curtime + 0.10f);
	}
	else
	{
		m_flScaleX = 1.0f;
		m_flScaleY = 1.0f;
		m_flScaleZ = 1.0f;
	}

	m_flFinalSize = 0.0f;

	UseClientSideAnimation();

	// Before we just used a rendercolor. Now we are taking more control over this.

	if (m_bUseRandomColor)
	{
		RandomColor();
	}

	else if (!m_bSetTemplated)
	{
		// If we are not using random colors, use this.
		m_clrRender = m_clrBox;
	}

	//Shadows look uggers when we scale up. 
	AddEffects (EF_NOSHADOW);
	BaseClass::Spawn();
}

void CPuzzleBox::ThinkFadeIn()
{
	DevMsg("Adding CPuzzleBox: Fading in\n");
//	m_flFinalSize = 0.2;
	m_flScaleX = 1;
	m_flScaleY = 1;
	m_flScaleZ = 1;

	m_flLerpTimeX = 1.0;
	m_flLerpTimeY = 1.0;
	m_flLerpTimeZ = 1.0;

	m_flGoalTimeX = gpGlobals->curtime;
	m_flGoalTimeY = gpGlobals->curtime;
	m_flGoalTimeZ = gpGlobals->curtime;

//	RemoveEffects(EF_NOSHADOW);
	SetThink( NULL );
}

void CPuzzleBox::InputSetColor( inputdata_t &inputdata )
{
	m_clrRender = inputdata.value.Color32();
}

void CPuzzleBox::InputRandomColor( inputdata_t &inputdata )
{
	RandomColor();
}

void CPuzzleBox::RandomColor()
{
	int iColorR= random->RandomInt(10,255);
	int iColorG= random->RandomInt(10,255);
	int iColorB= random->RandomInt(10,255);
	this->SetRenderColor( iColorR, iColorG, iColorB, 255 );
}

void CPuzzleBox::InputDissolve( inputdata_t &inputData )
{
	// We may have to think for this...
	m_OnDissolved.FireOutput( this, this );
	OnDissolve();
}

//-----------------------------------------------------------------------------
// Purpose: Pick me up!
//-----------------------------------------------------------------------------
int CPuzzleBox::ObjectCaps()
{ 
	int caps = BaseClass::ObjectCaps();

	if ( CBasePlayer::CanPickupObject( this, 45, 128 ) )
	{
		caps |= FCAP_IMPULSE_USE;
	}

	return caps;
}
void CPuzzleBox::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if (m_bShrinking)
		return;

	DevMsg ("+USE WAS PRESSED ON ME\n");
	CBasePlayer *pPlayer = ToBasePlayer( pActivator );
	if ( pPlayer )
	{
		if ( HasSpawnFlags( SF_PHYSPROP_ENABLE_PICKUP_OUTPUT ) )
		{
			m_OnPlayerUse.FireOutput( this, this );
		}
		pPlayer->PickupObject( this );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPuzzleBox::OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason )
{
	m_hPhysicsAttacker = pPhysGunUser;

	if ( reason == PICKED_UP_BY_CANNON || reason == PICKED_UP_BY_PLAYER )
	{
		m_OnPlayerPickup.FireOutput( pPhysGunUser, this );
	}

	if ( reason == PICKED_UP_BY_CANNON )
	{
		m_OnPhysGunPickup.FireOutput( pPhysGunUser, this );
	}
}
void CPuzzleBox::OnPhysGunDrop( CBasePlayer *pPhysGunUser, PhysGunDrop_t reason )
{
	m_OnPhysGunDrop.FireOutput( pPhysGunUser, this );
}

CBaseEntity* MakePlane ( CBaseEntity* pEnt )
{
	CBaseEntity *pRetVal = NULL;
	pRetVal = CreateEntityByName( "prop_dynamic" );
	pRetVal->SetModel("models/props_shapes/plane.mdl");
	pRetVal->SetCollisionGroup( COLLISION_GROUP_PUNTABLE );
	pRetVal->SetAbsOrigin( pEnt->GetAbsOrigin() );
//	pRetVal->SetAbsAngles( 0, 0, 0 );
//	pRetVal->SetAbsAngles( pEnt->GetAbsAngles() );
	QAngle vecAngles(0, pRetVal->GetAbsAngles().y, 0);
	pRetVal->SetAbsAngles(vecAngles);
//	pRetVal->SetOwnerEntity(pEnt);
//	pRetVal->Spawn();
	pRetVal->AddEffects (EF_NODRAW);
	pRetVal->VPhysicsInitNormal( SOLID_VPHYSICS, 0, false );
	pRetVal->VPhysicsGetObject()->EnableMotion( false );
	return pRetVal;
}

void CPuzzleBox::BecomePlane()
{
	if (m_bIsPlane)
		return;
		
//		QAngle vecAngles(0, GetAbsAngles().y, 0);

		//36x36x36 = 120x120x1
		// 120 / 36 = 3.3 //3.5
		// 1 / 36 =  0.027
		m_flScaleX = 3.3;
		m_flScaleY = 3.3;
		m_flScaleZ = 0.027;

		m_flLerpTimeX = 1.0;
		m_flLerpTimeY = 1.0;
		m_flLerpTimeZ = 1.0;

		m_flGoalTimeX = gpGlobals->curtime;
		m_flGoalTimeY = gpGlobals->curtime;
		m_flGoalTimeZ = gpGlobals->curtime;

		VPhysicsGetObject()->EnableGravity( false );
		VPhysicsGetObject()->EnableMotion( false );
		VPhysicsGetObject()->EnableCollisions( false );

		SetSolid( SOLID_NONE );
		MakePlanez();
		m_bIsPlane = true;
}

void CPuzzleBox::MakePlanez()
{
	CBaseEntity *pPlane = MakePlane( this );

	if (!m_bMadePlane)
	{
		pPlane->Spawn();
		SetOwnerEntity(pPlane);
		m_bIsPlane = true;
	}

}

void CPuzzleBox::OnDissolve()
{	
	if (m_bShrinking)
		return;

	CrHetoricalPlayer *pPlayer = (CrHetoricalPlayer *)GetPlayerHoldingEntity( this );
	if( pPlayer )
	{
		// Modify the velocity for held objects so it gets away from the player
		pPlayer->ForceDropOfCarriedPhysObjects( this );

		pPlayer->GetAbsVelocity();
		pPlayer->GetAbsVelocity() + Vector( pPlayer->EyeDirection2D().x * 4.0f, pPlayer->EyeDirection2D().y * 4.0f, -32.0f );
	}

//	DispatchParticleEffect ("puzzlecube_death", PATTACH_ABSORIGIN_FOLLOW, this ); 

	SetUse( NULL );

	if (m_bIsPlane)
	{
		VPhysicsGetObject()->EnableMotion( false );
		VPhysicsGetObject()->EnableCollisions( true );
		variant_t emptyVariant;
		GetOwnerEntity()->AcceptInput( "kill", this, this, emptyVariant, 0 );
		SetSolid( SOLID_NONE );
	}
	else
	{
		VPhysicsGetObject()->EnableGravity( false );
	}

	if (m_bSetTemplated)
	{
		variant_t emptyVariant;
	//	GetOwnerEntity()->AcceptInput( "spawnbox", this, this, emptyVariant, 0 );
	}

	Shrink();

}

void CPuzzleBox::Shrink()
{
	m_bShrinking = true;
	m_flFinalSize = 0.0;
	m_flScaleX = m_flFinalSize;
	m_flScaleY = m_flFinalSize;
	m_flScaleZ = m_flFinalSize;

	m_flLerpTimeX = 1.0;
	m_flLerpTimeY = 1.0;
	m_flLerpTimeZ = 1.0;

	m_flGoalTimeX = gpGlobals->curtime;
	m_flGoalTimeY = gpGlobals->curtime;
	m_flGoalTimeZ = gpGlobals->curtime;

	SetThink( &CPuzzleBox::ShrinkKill );
	SetNextThink(gpGlobals->curtime + 1.1f);
}

void CPuzzleBox::ShrinkKill()
{
	DevMsg("Removed CPuzzleBox from world\n");
	UTIL_Remove (this);
}

//****************************************************************************************************
//****************************************************************************************************
class CPointPuzzleBoxSpawn : public CLogicalEntity
{
	public: 	
	
	DECLARE_CLASS( CPointPuzzleBoxSpawn, CLogicalEntity);

	void Spawn( );
	void Precache( void );

	void SpawnBox( void );

	DECLARE_DATADESC();

	// Input functions.
	void InputSpawnBox( inputdata_t &inputData);

	void InputEnable( inputdata_t &inputdata );
	void InputDisable( inputdata_t &inputdata );
	void InputToggle( inputdata_t &inputdata );

	void InputSetColor( inputdata_t &inputData);

	void InputEnableRandomColor( inputdata_t &inputData);
	void InputDisableRandomColor( inputdata_t &inputData);
	void InputToggleRandomColor( inputdata_t &inputData);



private:
	bool m_bUseRandomColor;
	bool m_bDisabled;

	float		m_flConeDegrees;

	color32				m_clrSetBox;


	COutputEvent m_OnPostSpawnBox;

};

LINK_ENTITY_TO_CLASS( point_puzzle_box_spawn, CPointPuzzleBoxSpawn);

BEGIN_DATADESC( CPointPuzzleBoxSpawn )

	//Save/load
	DEFINE_KEYFIELD(m_bDisabled, FIELD_BOOLEAN, "StartDisabled"),
	DEFINE_INPUTFUNC(FIELD_VOID, "Enable", InputEnable),
	DEFINE_INPUTFUNC(FIELD_VOID, "Disable", InputDisable),
	DEFINE_INPUTFUNC(FIELD_VOID, "Toggle", InputToggle),

	DEFINE_INPUTFUNC( FIELD_VOID, "SpawnBox", InputSpawnBox ),

	DEFINE_KEYFIELD( m_clrSetBox, FIELD_COLOR32, "boxcolor" ),
	DEFINE_KEYFIELD( m_bUseRandomColor, FIELD_BOOLEAN, "randomcolor" ),
	DEFINE_INPUTFUNC( FIELD_VOID, "SetColor", InputSetColor ),

	DEFINE_INPUTFUNC(FIELD_VOID, "EnableRandomColors", InputEnableRandomColor),
	DEFINE_INPUTFUNC(FIELD_VOID, "DisableRandomColors", InputDisableRandomColor),
	DEFINE_INPUTFUNC(FIELD_VOID, "ToggleRandomColors", InputToggleRandomColor),

	DEFINE_OUTPUT( m_OnPostSpawnBox, "OnPostSpawnBox" ),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPointPuzzleBoxSpawn::Spawn( void )
{
	Precache();
	Think();
	BaseClass::Spawn();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPointPuzzleBoxSpawn::Precache( void )
{
	BaseClass::Precache();
}

//------------------------------------------------------------------------------
// Purpose: Turns on the relay, allowing it to fire outputs.
//------------------------------------------------------------------------------
void CPointPuzzleBoxSpawn::InputEnable( inputdata_t &inputdata )
{
	m_bDisabled = false;
}

//------------------------------------------------------------------------------
// Purpose: Turns on the relay, allowing it to fire outputs.
//------------------------------------------------------------------------------
void CPointPuzzleBoxSpawn::InputDisable( inputdata_t &inputdata )
{
	m_bDisabled = true;
}

//------------------------------------------------------------------------------
// Purpose: Turns on the relay, allowing it to fire outputs.
//------------------------------------------------------------------------------
void CPointPuzzleBoxSpawn::InputToggle( inputdata_t &inputdata )
{
	m_bDisabled = !m_bDisabled;
}

//-----------------------------------------------------------------------------
// Purpose: "Then I flip a switch..."
//-----------------------------------------------------------------------------
void CPointPuzzleBoxSpawn::InputSpawnBox( inputdata_t &inputData)
{
	m_OnPostSpawnBox.FireOutput( inputData.pActivator, this );
	SpawnBox();

}

//-----------------------------------------------------------------------------
// Purpose: Colors!!!
//-----------------------------------------------------------------------------
void CPointPuzzleBoxSpawn::InputSetColor( inputdata_t &inputdata )
{
	// We disable the random color when we force a color.
	if (m_bUseRandomColor)
	{
		DevMsg("CPointPuzzleBoxSpawn: Now turning off random colors.\n");
		m_bUseRandomColor = false;
	}

	m_clrRender = inputdata.value.Color32();
}

void CPointPuzzleBoxSpawn::InputEnableRandomColor( inputdata_t &inputdata )
{
	// We disable the random color when we force a color.
//	if (!m_bUseRandomColor)
//	{
		DevMsg("CPointPuzzleBoxSpawn: Now turning on random colors.\n");
		m_bUseRandomColor = true;
//	}
}

void CPointPuzzleBoxSpawn::InputDisableRandomColor( inputdata_t &inputdata )
{
	// We disable the random color when we force a color.
//	if (m_bUseRandomColor)
//	{
		DevMsg("CPointPuzzleBoxSpawn: Now turning off random colors.\n");
		m_bUseRandomColor = false;
//	}
}

void CPointPuzzleBoxSpawn::InputToggleRandomColor( inputdata_t &inputdata )
{
	m_bUseRandomColor = !m_bUseRandomColor;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPointPuzzleBoxSpawn::SpawnBox()
{
	CPuzzleBox *pBox = static_cast<CPuzzleBox*>( CreateEntityByName( "prop_puzzle_box" ) );

	if ( pBox == NULL )
		 return;

	Vector vecAbsOrigin = GetAbsOrigin();
	Vector zaxis;
	
	pBox->SetAbsOrigin( vecAbsOrigin );

	Vector vDirection;
	QAngle qAngle = GetAbsAngles();

	qAngle = qAngle + QAngle ( random->RandomFloat( -m_flConeDegrees, m_flConeDegrees ), random->RandomFloat( -m_flConeDegrees, m_flConeDegrees ), 0 );

	AngleVectors( qAngle, &vDirection, NULL, NULL );

	//OMG LOL IF YOU JUMP ON THE BOXES WITH THIS, YOU FLY
	//vDirection *= 1000.0f;
	//pBox->SetAbsVelocity( vDirection );

//	GetOwnerEntity()->SetAbsAngles(qAngles);

	if (m_bUseRandomColor)
	{
		pBox->UseRandomColor( true );
	}
	else
	{	// Fix me. When we go back to userandom, it fucks
		// If we are not using random colors, use this.
		pBox->UseRandomColor( false );
		pBox->m_clrRender = m_clrSetBox;
	}
	pBox->SetTemplated( true );
//	pBox->SetOwnerEntity(this);
	pBox->FadeIn( true );
	pBox->Spawn();

}


