//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "iclientmode.h"
#include "engine/IEngineSound.h"
#include "vgui_controls/AnimationController.h"
#include "vgui_controls/Controls.h"
#include "vgui_controls/Panel.h"
#include "vgui/ISurface.h"
#include "../hud_crosshair.h"

#ifdef SIXENSE
#include "sixense/in_sixense.h"
#include "view.h"
int ScreenTransform( const Vector& point, Vector& screen );
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static ConVar hud_quickinfo( "crosshair", "1", FCVAR_ARCHIVE );
static ConVar quickinfo_dynamic ( "crosshair_dynamic", "1", FCVAR_ARCHIVE );
static ConVar c_puntgun_highlight( "c_puntgun_highlight", "0", FCVAR_HIDDEN );
ConVar hud_quickinfo_alignment( "hud_crosshair_alignment_x", "31", FCVAR_ARCHIVE );
extern ConVar crosshair;

#define QUICKINFO_EVENT_DURATION	1.0f
#define	QUICKINFO_BRIGHTNESS_FULL	255
#define	QUICKINFO_BRIGHTNESS_DIM	255 // 64
#define	QUICKINFO_FADE_IN_TIME		0.5f
#define QUICKINFO_FADE_OUT_TIME		0.5f // 2.0

/*
==================================================
CHUDQuickInfo 
==================================================
*/

using namespace vgui;

class CHUDQuickInfo : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHUDQuickInfo, vgui::Panel );
public:
	CHUDQuickInfo( const char *pElementName );
	void Init( void );
	void VidInit( void );
	bool ShouldDraw( void );
	virtual void OnThink();
	virtual void Paint();
	
	virtual void ApplySchemeSettings( IScheme *scheme );
private:
	
	void	DrawWarning( int x, int y, CHudTexture *icon, float &time );
//	void	DrawUpdate( int x, int y, CHudTexture *icon, float &time );
	void	UpdateEventTime( void );
	bool	EventTimeElapsed( void );

	bool	m_warnXhair;
	float   m_XhairFade;

	bool	m_bFadedOut;
	
	bool	m_bDimmed;			// Whether or not we are dimmed down
	float	m_flLastEventTime;	// Last active event (controls dimmed state)

	CHudTexture	*m_icon_c;

	CHudTexture	*m_icon_empty;	
	CHudTexture	*m_icon_full;		
};

DECLARE_HUDELEMENT( CHUDQuickInfo );

CHUDQuickInfo::CHUDQuickInfo( const char *pElementName ) :
	CHudElement( pElementName ), BaseClass( NULL, "HUDQuickInfo" )
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	SetHiddenBits( HIDEHUD_CROSSHAIR );
}

void CHUDQuickInfo::ApplySchemeSettings( IScheme *scheme )
{
	BaseClass::ApplySchemeSettings( scheme );

	SetPaintBackgroundEnabled( false );
	SetForceStereoRenderToFrameBuffer( true );
}


void CHUDQuickInfo::Init( void )
{
	m_XhairFade	= 255.0f;

	m_warnXhair	= false;

	m_bFadedOut			= false;
	m_bDimmed			= false;
	m_flLastEventTime   = 0.0f;
}


void CHUDQuickInfo::VidInit( void )
{
	Init();

	m_icon_c = gHUD.GetIcon( "crosshair" );
	m_icon_full = gHUD.GetIcon( "Punt_Crosshair_full" );
	m_icon_empty = gHUD.GetIcon( "Punt_Crosshair_empty" );

}


void CHUDQuickInfo::DrawWarning( int x, int y, CHudTexture *icon, float &time )
{
	float scale	= (int)( fabs(sin(gpGlobals->curtime*8.0f)) * 128.0);

	// Only fade out at the low point of our blink
	if ( time <= (gpGlobals->frametime * 200.0f) )
	{
		if ( scale < 40 )
		{
			time = 0.0f;
			return;
		}
		else
		{
			// Counteract the offset below to survive another frame
			time += (gpGlobals->frametime * 200.0f);
		}
	}
	
	// Update our time
	time -= (gpGlobals->frametime * 200.0f);
	Color caution = gHUD.m_clrCaution;
	caution[3] = scale * 255;

	icon->DrawSelf( x, y, caution );
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHUDQuickInfo::ShouldDraw( void )
{
	if ( !m_icon_c || !m_icon_empty || !m_icon_full) 
		return false;

	C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
	if ( player == NULL )
		return false;

//	if ( crosshair.GetBool() && !IsX360() )
//		return false;

	return ( CHudElement::ShouldDraw() && !engine->IsDrawingLoadingImage() );
}

//-----------------------------------------------------------------------------
// Purpose: Checks if the hud element needs to fade out
//-----------------------------------------------------------------------------
void CHUDQuickInfo::OnThink()
{
	BaseClass::OnThink();

	C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
	if ( player == NULL )
		return;

	// see if we should fade in/out
	bool bFadeOut = player->IsZoomed();

	// check if the state has changed
	if ( m_bFadedOut != bFadeOut )
	{
		m_bFadedOut = bFadeOut;

		m_bDimmed = false;
/*
		if ( bFadeOut )
		{
			g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "Alpha", 0.0f, 0.0f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		}
		else
		{
			g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "Alpha", QUICKINFO_BRIGHTNESS_FULL, 0.0f, 0.25f, vgui::AnimationController::INTERPOLATOR_LINEAR );
		}
*/
	}
	else if ( !m_bFadedOut )
	{
		// If we're dormant, fade out
		if ( EventTimeElapsed() )
		{
			if ( !m_bDimmed )
			{
				m_bDimmed = false;
				g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "Alpha", QUICKINFO_BRIGHTNESS_DIM, 0.0f, QUICKINFO_FADE_OUT_TIME, vgui::AnimationController::INTERPOLATOR_LINEAR );
			}
		}
		else if ( m_bDimmed )
		{
			// Fade back up, we're active
			m_bDimmed = false;
			g_pClientMode->GetViewportAnimationController()->RunAnimationCommand( this, "Alpha", QUICKINFO_BRIGHTNESS_FULL, 0.0f, QUICKINFO_FADE_IN_TIME, vgui::AnimationController::INTERPOLATOR_LINEAR );
		}
	}
}

void CHUDQuickInfo::Paint()
{
	C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
	if ( player == NULL )
		return;

	C_BaseCombatWeapon *pWeapon = GetActiveWeapon();
	if ( pWeapon == NULL )
		return;

	float fX, fY;
	bool bBehindCamera = false;
	CHudCrosshair::GetDrawPosition( &fX, &fY, &bBehindCamera );

	// if the crosshair is behind the camera, don't draw it
	if( bBehindCamera )
		return;

//	int		xCenter	= ( ScreenWidth() - m_icon_c->Width() ) / 2;
//	int		yCenter = ( ScreenHeight() - m_icon_c->Height() ) / 2;

	int		xCenter	= (int)fX - m_icon_c->Width()  / 2;
	int		yCenter = (int)fY -  m_icon_c->Height()  / 2;

	float	scalar  = 255.0f/255.0f; //138.0f/255.0f;
	
//	m_icon_c->DrawSelf( xCenter, yCenter, clrNormal );

	// adjust center for the bigger crosshairs
//	xCenter	= ScreenWidth() / 2;
//	yCenter = ( ScreenHeight() - m_icon_full->Height() ) / 2;

	Color clrNormal = gHUD.m_clrNormal;
	clrNormal[3] = 255 * scalar;

	if( IsX360() )
	{
		// Because the fixed reticle draws on half-texels, this rather unsightly hack really helps
		// center the appearance of the quickinfo on 360 displays.
		xCenter += 1;
	}

	if ( !hud_quickinfo.GetInt() )
		return;

	int	sinScale = (int)( fabs(sin(gpGlobals->curtime*8.0f)) * 128.0f );

	Color XhairColor = m_warnXhair ? gHUD.m_clrNormal : gHUD.m_clrNormal;;	

	if ( m_warnXhair )
	{
		XhairColor[3] = 255 * sinScale;
	}
	else
	{
		XhairColor[3] = 255 * scalar;
	}
	if (quickinfo_dynamic.GetBool())
	{
		//ConVar *c_puntgun_highlight= cvar->FindVar( "c_puntgun_highlight" );
		if (c_puntgun_highlight.GetBool())
		{
			gHUD.DrawIconProgressBar( xCenter - (m_icon_full->Width() - hud_quickinfo_alignment.GetInt()), yCenter, m_icon_full, m_icon_empty, ( 1.0f - 100.0f ), XhairColor, CHud::HUDPB_VERTICAL );
			//gHUD.DrawIconProgressBar( xCenter - (m_icon_lb->Width() * 2), yCenter, m_icon_lb, m_icon_lbe, ( 1.0f - 100.0f ), XhairColor, CHud::HUDPB_VERTICAL );
			//gHUD.DrawIconProgressBar( xCenter + m_icon_rb->Width(), yCenter, m_icon_rb, m_icon_rbe, ( 1.0f - 100.0f ), XhairColor, CHud::HUDPB_VERTICAL );
		}	
		else 
		{
			gHUD.DrawIconProgressBar( xCenter - (m_icon_full->Width() - hud_quickinfo_alignment.GetInt()), yCenter, m_icon_full, m_icon_empty, ( 1.0f - 0.0f ), XhairColor, CHud::HUDPB_VERTICAL );
			//gHUD.DrawIconProgressBar( xCenter - (m_icon_lb->Width() * 2), yCenter, m_icon_lb, m_icon_lbe, ( 1.0f - 0.0f ), XhairColor, CHud::HUDPB_VERTICAL );
			//gHUD.DrawIconProgressBar( xCenter + m_icon_rb->Width(), yCenter, m_icon_rb, m_icon_rbe, ( 1.0f - 0.0f ), XhairColor, CHud::HUDPB_VERTICAL );
		}
	}
	else
	{
			gHUD.DrawIconProgressBar( xCenter - (m_icon_full->Width() - hud_quickinfo_alignment.GetInt()), yCenter, m_icon_full, m_icon_empty, ( 1.0f - 100.0f ), XhairColor, CHud::HUDPB_VERTICAL );
			//gHUD.DrawIconProgressBar( xCenter - (m_icon_lb->Width() * 2), yCenter, m_icon_lb, m_icon_lbe, ( 1.0f - 100.0f ), XhairColor, CHud::HUDPB_VERTICAL );
			//gHUD.DrawIconProgressBar( xCenter + m_icon_rb->Width(), yCenter, m_icon_rb, m_icon_rbe, ( 1.0f - 100.0f ), XhairColor, CHud::HUDPB_VERTICAL );
	}


}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHUDQuickInfo::UpdateEventTime( void )
{
	m_flLastEventTime = gpGlobals->curtime;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CHUDQuickInfo::EventTimeElapsed( void )
{
	if (( gpGlobals->curtime - m_flLastEventTime ) > QUICKINFO_EVENT_DURATION )
		return true;

	return false;
}

