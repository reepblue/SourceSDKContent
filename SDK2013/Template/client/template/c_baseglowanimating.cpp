//=========== Copyright © 2014, rHetorical, All rights reserved. =============
//
// Purpose: 
//		
//=============================================================================

#include "cbase.h"
#include "c_baseglowanimating.h"
#include "c_baseanimating.h"
#include "glow_outline_effect.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

EXTERN_RECV_TABLE(DT_BaseGlowAnimating);
IMPLEMENT_CLIENTCLASS(C_BaseGlowAnimating, DT_BaseGlowAnimating, CBaseGlowAnimating);

BEGIN_RECV_TABLE(C_BaseGlowAnimating, DT_BaseGlowAnimating)
	RecvPropBool( RECVINFO( m_bGlowEnabled ) ),
	RecvPropFloat( RECVINFO( m_flRedGlowColor ) ),
	RecvPropFloat( RECVINFO( m_flGreenGlowColor ) ),
	RecvPropFloat( RECVINFO( m_flBlueGlowColor ) ),
END_RECV_TABLE()

BEGIN_PREDICTION_DATA( C_BaseGlowAnimating )
END_PREDICTION_DATA()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_BaseGlowAnimating::C_BaseGlowAnimating()
{
	m_pGlowEffect = NULL;
	m_bGlowEnabled = false;
	m_bOldGlowEnabled = false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_BaseGlowAnimating::~C_BaseGlowAnimating()
{
	DestroyGlowEffect();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_BaseGlowAnimating::OnPreDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnPreDataChanged( updateType );

	m_bOldGlowEnabled = m_bGlowEnabled;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_BaseGlowAnimating::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );

	if ( m_bOldGlowEnabled != m_bGlowEnabled )
	{
		UpdateGlowEffect();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_BaseGlowAnimating::GetGlowEffectColor( float *r, float *g, float *b )
{
	*r = m_flRedGlowColor;
	*g = m_flGreenGlowColor;
	*b = m_flBlueGlowColor;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_BaseGlowAnimating::UpdateGlowEffect( void )
{
	// destroy the existing effect
	if ( m_pGlowEffect )
	{
		DestroyGlowEffect();
	}

	// create a new effect
	if ( m_bGlowEnabled )
	{
		float r, g, b;
		GetGlowEffectColor( &r, &g, &b );

		m_pGlowEffect = new CGlowObject( this, Vector( r, g, b ), 1.0, true );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_BaseGlowAnimating::DestroyGlowEffect( void )
{
	if ( m_pGlowEffect )
	{
		delete m_pGlowEffect;
		m_pGlowEffect = NULL;
	}
}