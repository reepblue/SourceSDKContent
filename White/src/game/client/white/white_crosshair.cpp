//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "iclientmode.h"
#include "engine/IEngineSound.h"
#include "vgui_controls/AnimationController.h"
#include "vgui_controls/Controls.h"
#include <vgui_controls/Panel.h>
#include <vgui/isurface.h>
#include "clientmode.h"
#include "white_crosshair.h"
#include "c_white_player.h"
#include "materialsystem/IMaterial.h"
#include "materialsystem/IMesh.h"
#include "materialsystem/imaterialvar.h"
#include "mathlib/mathlib.h"
#include "../hud_crosshair.h"

#ifdef SIXENSE
#include "sixense/in_sixense.h"
#include "view.h"
int ScreenTransform( const Vector& point, Vector& screen );
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static ConVar c_puntgun_highlight( "c_puntgun_highlight", "0", FCVAR_HIDDEN );
ConVar cl_crosshair_red( "cl_crosshair_red", "10", FCVAR_ARCHIVE );
ConVar cl_crosshair_green( "cl_crosshair_green", "10", FCVAR_ARCHIVE );
ConVar cl_crosshair_blue( "cl_crosshair_blue", "10", FCVAR_ARCHIVE );
ConVar cl_crosshair_alpha( "cl_crosshair_alpha", "200", FCVAR_ARCHIVE );

//ConVar cl_crosshair_file( "cl_crosshair_file", "crosshair1", FCVAR_ARCHIVE );

ConVar cl_crosshair_scale( "cl_crosshair_scale", "32.0", FCVAR_ARCHIVE );

using namespace vgui;

DECLARE_HUDELEMENT( CHudWhiteCrosshair );

CHudWhiteCrosshair::CHudWhiteCrosshair( const char *pName ) :
	vgui::Panel( NULL, "HudWhiteCrosshair" ), CHudElement( pName )
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	SetHiddenBits( HIDEHUD_PLAYERDEAD );

	m_szPreviousCrosshair[0] = '\0';
	m_flAccuracy = 0.1;
}

void CHudWhiteCrosshair::ApplySchemeSettings( IScheme *scheme )
{
	BaseClass::ApplySchemeSettings( scheme );

	m_icon_full = gHUD.GetIcon( "Punt_Crosshair_full" );
	m_icon_empty = gHUD.GetIcon( "Punt_Crosshair_empty");

	SetPaintBackgroundEnabled( false );

	SetSize( ScreenWidth(), ScreenHeight() );

	SetForceStereoRenderToFrameBuffer( true );
}

void CHudWhiteCrosshair::LevelShutdown( void )
{
	// forces m_pFrameVar to recreate next map
	m_szPreviousCrosshair[0] = '\0';

	if ( m_pCrosshair )
	{
		delete m_pCrosshair;
		m_pCrosshair = NULL;
		crosshair.SetValue(1);
	}
}

void CHudWhiteCrosshair::Init()
{
	//m_iCrosshairTextureID = vgui::surface()->CreateNewTextureID();
}

bool CHudWhiteCrosshair::ShouldDraw()
{
	C_rHetoricalPlayer *pPlayer = C_rHetoricalPlayer::GetLocalPlayer();
	if ( !pPlayer )
		return false;

	return true;
}

void CHudWhiteCrosshair::Paint()
{
	C_rHetoricalPlayer *pPlayer = C_rHetoricalPlayer::GetLocalPlayer();
	if( !pPlayer )
		return;

	float fX, fY;
	bool bBehindCamera = false;
	CHudCrosshair::GetDrawPosition( &fX, &fY, &bBehindCamera );

	// if the crosshair is behind the camera, don't draw it
	if( bBehindCamera )
		return;

/*
	const char *crosshairfile = cl_crosshair_file.GetString();
	if ( !crosshairfile )
		return;

	if ( Q_stricmp( m_szPreviousCrosshair, crosshairfile ) != 0 )
	{
		char buf[256];
		Q_snprintf( buf, sizeof(buf), "vgui/crosshairs/%s", crosshairfile );

		vgui::surface()->DrawSetTextureFile( m_iCrosshairTextureID, buf, true, false );

		if ( m_pCrosshair )
		{
			delete m_pCrosshair;
		}

		m_pCrosshair = vgui::surface()->DrawGetTextureMatInfoFactory( m_iCrosshairTextureID );

		if ( !m_pCrosshair )
			return;

		// save the name to compare with the cvar in the future
		Q_strncpy( m_szPreviousCrosshair, crosshairfile, sizeof(m_szPreviousCrosshair) );
	}

	Color clr( cl_crosshair_red.GetInt(), cl_crosshair_green.GetInt(), cl_crosshair_blue.GetInt(), 255 );

	int screenWide, screenTall;
	GetHudSize(screenWide, screenTall);

	int iX = screenWide / 2;
	int iY = screenTall / 2;

	int iWidth, iHeight;

	iWidth = iHeight = cl_crosshair_scale.GetInt();

	vgui::surface()->DrawSetColor( clr );
	vgui::surface()->DrawSetTexture( m_iCrosshairTextureID );
	vgui::surface()->DrawTexturedRect( iX-iWidth, iY-iHeight, iX+iWidth, iY+iHeight );
	vgui::surface()->DrawSetTexture(0);
*/
	float flPlayerScale = 1.0f;
	float flWeaponScale = 1.0f;
	int iTextureW = m_icon_empty->Width();
	int iTextureH = m_icon_empty->Height();

	int iTextureFullW = m_icon_full->Width();
	int iTextureFullH = m_icon_full->Height();

	float flWidth = flWeaponScale * flPlayerScale * (float)iTextureW;
	float flHeight = flWeaponScale * flPlayerScale * (float)iTextureH;
	int iWidth = (int)( flWidth + 0.5f );
	int iHeight = (int)( flHeight + 0.5f );
	int iX = (int)( fX + 0.5f );
	int iY = (int)( fY + 0.5f );

	Color clr( cl_crosshair_red.GetInt(), cl_crosshair_green.GetInt(), cl_crosshair_blue.GetInt(), 255 );

	if (c_puntgun_highlight.GetBool())
	{
		m_icon_full->DrawSelfCropped (
			iX-(iWidth/2), iY-(iHeight/2),
			0, 0,
			iTextureFullW, iTextureFullH,
			iWidth, iHeight,
			clr );
	}
	else
	{
		m_icon_empty->DrawSelfCropped (
			iX-(iWidth/2), iY-(iHeight/2),
			0, 0,
			iTextureW, iTextureH,
			iWidth, iHeight,
			clr );
	}
}

