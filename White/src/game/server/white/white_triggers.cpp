//=========== Copyright © 2013, rHetorical, All rights reserved. =============
//
// Purpose:
//		
//=============================================================================

#include "cbase.h"
#include "weapon_physcannon.h"
#include "white_player.h"
#include <convar.h>
#include "saverestore_utlvector.h"
#include "triggers.h"
#include "physics.h"
#include "prop_puzzle_box.h"
#include "vphysics_interface.h"

//-----------------------------------------------------------------------------
// Launcher Trigger
//-----------------------------------------------------------------------------
class CTriggerLauncher : public CBaseTrigger
{
	    DECLARE_CLASS(CTriggerLauncher, CBaseTrigger);
        DECLARE_DATADESC();

public:

        void Spawn();
        virtual void StartTouch(CBaseEntity *pOther);

		const char *m_szTarget;
        float m_fHeight;
		float m_fSpeed;

        CBaseEntity *m_pTarget;

private:
		void InputEnable( inputdata_t &inputdata );
		void InputDisable( inputdata_t &inputdata );
		bool m_bDisabled;
};

LINK_ENTITY_TO_CLASS(trigger_launcher, CTriggerLauncher);

BEGIN_DATADESC(CTriggerLauncher)
        DEFINE_ENTITYFUNC(StartTouch),

        DEFINE_KEYFIELD(m_szTarget, FIELD_STRING, "target"),
        DEFINE_KEYFIELD(m_fHeight, FIELD_FLOAT, "height"),
		DEFINE_KEYFIELD(m_fSpeed, FIELD_FLOAT, "speed"),
END_DATADESC()

void CTriggerLauncher::Spawn()
{
	BaseClass::Spawn();
        
	SetSolid(SOLID_VPHYSICS);
	VPhysicsInitShadow(false, false);
	SetModel(STRING(GetModelName()));
	SetMoveType(MOVETYPE_NONE);
	SetRenderMode(kRenderNone);
}

void CTriggerLauncher::StartTouch(CBaseEntity *pOther)
{
	if ( !PassesTriggerFilters(pOther) )
		return;

	if (m_bDisabled)
		return;

	if(m_pTarget == 0)
	{
		m_pTarget = gEntList.FindEntityByName(0, m_szTarget);

		if(m_pTarget == 0)
			return;
	}

//      if(pOther->IsPlayer() == true)
//      {
				Vector vecToTarget = (m_pTarget->GetAbsOrigin() - pOther->GetAbsOrigin());
                vecToTarget = vecToTarget * 0.5f;
                vecToTarget.z = m_fHeight * 3.0f;

                /*Vector out;
                VMatrix mat;
                mat.Identity();
                MatrixRotate(mat, Vector(1.0f, 0.0f, 0.0f), 30.0f);
                VectorRotate(toTarget, mat.As3x4(), out);*/
                
                //VectorNormalizeFast(toTarget);
               // DevMsg("toTarget: %f %f %f\n", toTarget.x, toTarget.y, toTarget.z);

                //toTarget = toTarget * trigger_jump_speed.GetFloat();

                pOther->SetAbsVelocity(Vector(0, 0, 0));
                pOther->SetBaseVelocity(vecToTarget * m_fSpeed);
//        }

}

void CTriggerLauncher::InputEnable( inputdata_t &inputdata )
{
	m_bDisabled = false;
}

void CTriggerLauncher::InputDisable( inputdata_t &inputdata )
{
	m_bDisabled = true;
}

class CPointBoxConverter : public CLogicalEntity
{
	public: 	
	
	DECLARE_CLASS( CPointBoxConverter, CLogicalEntity);

	void Spawn( );
	void Precache( void );

	DECLARE_DATADESC();

	// Input functions.
	void InputConvertBox( inputdata_t &inputData);

	void InputEnable( inputdata_t &inputdata );
	void InputDisable( inputdata_t &inputdata );
	void InputToggle( inputdata_t &inputdata );


/*
enum FlattenSide_t
{
	HORIZONTAL			= 0,	
	VERTICAL			= 1,	
};
*/
private:

	bool m_bDisabled;
	bool m_bUseAngles;
	int m_intSide;
	bool m_bHasBox;

	QAngle m_vSaveAngles;

	COutputEvent m_OnConvertBox;

};

LINK_ENTITY_TO_CLASS( point_puzzle_box_convert, CPointBoxConverter);

BEGIN_DATADESC( CPointBoxConverter )

	//Save/load
	DEFINE_KEYFIELD(m_bDisabled, FIELD_BOOLEAN, "StartDisabled"),
	DEFINE_KEYFIELD(m_bUseAngles, FIELD_BOOLEAN, "UseAngles"),
//	DEFINE_KEYFIELD(m_intSide, FIELD_INTEGER, "side"),
	DEFINE_INPUTFUNC(FIELD_VOID, "Enable", InputEnable),
	DEFINE_INPUTFUNC(FIELD_VOID, "Disable", InputDisable),
	DEFINE_INPUTFUNC(FIELD_VOID, "Toggle", InputToggle),

	DEFINE_INPUTFUNC( FIELD_STRING, "ConvertBox", InputConvertBox ),

	DEFINE_OUTPUT( m_OnConvertBox, "OnConvertBox" ),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPointBoxConverter::Spawn( void )
{
	Precache();
	Think();
	BaseClass::Spawn();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPointBoxConverter::Precache( void )
{
	BaseClass::Precache();
}

//------------------------------------------------------------------------------
// Purpose: Turns on the relay, allowing it to fire outputs.
//------------------------------------------------------------------------------
void CPointBoxConverter::InputEnable( inputdata_t &inputdata )
{
	m_bDisabled = false;
}

//------------------------------------------------------------------------------
// Purpose: Turns on the relay, allowing it to fire outputs.
//------------------------------------------------------------------------------
void CPointBoxConverter::InputDisable( inputdata_t &inputdata )
{
	m_bDisabled = true;
}

//------------------------------------------------------------------------------
// Purpose: Turns on the relay, allowing it to fire outputs.
//------------------------------------------------------------------------------
void CPointBoxConverter::InputToggle( inputdata_t &inputdata )
{
	m_bDisabled = !m_bDisabled;
}

//-----------------------------------------------------------------------------
// Purpose: "Then I flip a switch..."
//-----------------------------------------------------------------------------
void CPointBoxConverter::InputConvertBox( inputdata_t &inputdata)
{
	if (m_bDisabled)
		return;

	if (m_bHasBox)
		return;

	CBaseEntity *pTarget = gEntList.FindEntityByName( NULL, inputdata.value.String(), NULL, inputdata.pActivator, inputdata.pCaller );

	if ( pTarget->ClassMatches("prop_puzzle_box") )
	{
		//Get our entity.
		CPuzzleBox *pPuzzleBox = dynamic_cast<CPuzzleBox*>( pTarget );

		// Get THIS entity's angles
//		m_vSaveAngles = this->GetAbsAngles();
//		m_vSaveOrigin = this->GetAbsOrigin();


		// Apply the angles to the box and make the box into a plane.
		// The brush edit the box angles, then the box will use the new angles for the plane model.
		pPuzzleBox->BecomePlane();

		if (m_bUseAngles)
		{
			QAngle m_vSaveAngles(GetAbsAngles().x, GetAbsAngles().y, GetAbsAngles().z);
			pPuzzleBox->SetAbsAngles(m_vSaveAngles);
		}

		//We Own this entity now.
		this->SetOwnerEntity(pPuzzleBox);

		// Finally, tell the bool we've made a plane out of the box.
		m_bHasBox = true;
	}
	
	m_OnConvertBox.FireOutput( inputdata.pActivator, this );

}

//-----------------------------------------------------------------------------
// func_puzzle_box_convert
//-----------------------------------------------------------------------------
class CFuncConvert  : public CTriggerMultiple
{
public:
	DECLARE_CLASS( CFuncConvert, CTriggerMultiple  );
	DECLARE_DATADESC();
 
	void Spawn();
	void Touch( CBaseEntity *pOther );
	
private:

 	void InputEnable( inputdata_t &data );
	void InputDisable( inputdata_t &data );
	bool m_bDisabled;

	void InputDissolveCube( inputdata_t &data );
	bool m_bHasBox;

//	Vector m_vSaveOrigin;
//	QAngle m_vSaveAngles;

	color32	m_clr;
};

LINK_ENTITY_TO_CLASS( func_puzzle_box_convert, CFuncConvert );
 
BEGIN_DATADESC( CFuncConvert )

	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),

	DEFINE_KEYFIELD(m_bDisabled, FIELD_BOOLEAN, "StartDisabled"),
	DEFINE_KEYFIELD( m_clr, FIELD_COLOR32, "brushcolor" ),

	DEFINE_INPUTFUNC(FIELD_VOID, "DissolveCube", InputDissolveCube),
//	DEFINE_INPUTFUNC(FIELD_VOID, "SetNonSoild", InputSetNonSoild),
 
END_DATADESC()

void CFuncConvert::Spawn()
{
	BaseClass::Spawn();

	SetSolid( SOLID_NONE );

	SetEffects( kRenderFxPulseSlow );
 
	m_clrRender = m_clr;

	// Use our brushmodel
	SetModel( STRING( GetModelName() ) );
 
	AddFlag( FL_WORLDBRUSH );

}

void CFuncConvert::InputEnable( inputdata_t &inputdata )
{
	m_bDisabled = false;
	RemoveEffects( EF_NODRAW );
	SetSolid( SOLID_BSP );
	//VPhysicsGetObject()->EnableCollisions(true);
}

void CFuncConvert::InputDisable( inputdata_t &inputdata )
{
	m_bDisabled = true;
	AddEffects( EF_NODRAW );
	SetSolid( SOLID_NONE );
	//VPhysicsGetObject()->EnableCollisions(false);
}

void CFuncConvert::Touch( CBaseEntity *pOther )
{
	// If we are disabled, ignore.
	if (m_bDisabled)
		return;

	// If we already have a box, ignore.
	if(m_bHasBox)
		return;

	// Make sure it's a box.
	if ( pOther->ClassMatches("prop_puzzle_box") )
	{
		//Get the box entity.
		CPuzzleBox *pPuzzleBox = dynamic_cast<CPuzzleBox*>( pOther );
		// Remove it
		pPuzzleBox->BecomePlane();

		AddEffects( EF_NODRAW );
		// Finally, tell the bool we've made a plane out of the box.
		m_bHasBox = true;
	}
}

void CFuncConvert::InputDissolveCube ( inputdata_t &inputdata )
{
	variant_t emptyVariant;
	GetOwnerEntity()->AcceptInput( "Dissolve", this, this, emptyVariant, 0 );
	RemoveEffects( EF_NODRAW );
	m_bHasBox = false;
}