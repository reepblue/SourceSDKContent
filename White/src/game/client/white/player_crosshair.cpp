//#include "hud.h"
#include "cbase.h"
#include "c_white_player.h"
#include "player_crosshair.h"
#include "iclientmode.h"
#include "hud_macros.h"
#include "c_team.h"
#include "vgui_controls/controls.h"
#include "vgui/ISurface.h"
#include "tier0/memdbgon.h"
#include <vgui_controls/Panel.h>
#include "view.h"

using namespace vgui;
extern ConVar crosshair; 
DECLARE_HUDELEMENT( CEngineXhair );

//ConVar crosshair_size ("crosshair_size", "2" );

//-----------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------
CEngineXhair::CEngineXhair( const char *pElementName ) : CHudElement( pElementName ), BaseClass( NULL, "EngineXhair" )
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );
   
   SetVisible( true );
   SetEnabled( true );

   m_bUseThisXhair = true;
   // This defines m_nIraqIcon as "vgui/hud/teamicons/iraqiran" which calls the vtf.. duh.
   m_nxhair = surface()->CreateNewTextureID();
   surface()->DrawSetTexture( m_nxhair );
     m_clrCrosshair = Color( 255, 255, 255, 255 );
//  surface()->DrawSetTextureFile( m_nxhair, "vgui/hud/as_crosshair_01" , true, false);

   SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
}

//-----------------------------------------------------------
// Purpose: If pPlayer (me) is on team Combine, display Iraq icon
// Purpose+: else if i'm on team Rebels display British icon.
//-----------------------------------------------------------
void CEngineXhair::PaintBackground()
{  
	C_BasePlayer* pPlayer = C_rHetoricalPlayer::GetLocalPlayer();
	if ( !pPlayer )
		return;

	if ( !pPlayer->IsAlive() )
		return;

	if (!crosshair.GetBool()) 
		return;

	if (!m_bUseThisXhair)
		return;

   SetBgColor(Color(0,0,0,0));

  // Color(255, 255, 255, 255);
  // CBasePlayer *pPlayer = UTIL_PlayerByIndex(engine->GetLocalPlayer());

   	surface()->DrawSetColor( m_clrCrosshair );
	//surface()->DrawSetColor(GetFgColor());
	surface()->DrawSetTexture( m_nxhair );

//	surface()->DrawTexturedRect(crosshair_x.GetFloat() - crosshair_wide.GetFloat(), crosshair_y.GetFloat() - crosshair_wide.GetFloat(), crosshair_x.GetFloat() + crosshair_wide.GetFloat(), crosshair_y.GetFloat() +crosshair_wide.GetFloat());

	float x, y;
	x = ScreenWidth()/2;
	y = ScreenHeight()/2;

	m_curViewAngles = CurrentViewAngles();
	m_curViewOrigin = CurrentViewOrigin();

	Vector screen;
	screen.Init();

	int width = MAX( 1, y * 0.03 ); //0.03f
	int height = MAX( 1, y * 0.005 ); //0.005f

	surface()->DrawFilledRect( x - width - height,
		y - height / 2,
		x - width,
		y + height / 2 );

	surface()->DrawFilledRect( x + width,
		y - height / 2,
		x + width + height,
		y + height / 2 );

	surface()->DrawFilledRect( x - height / 2,
		y - width - height,
		x + height / 2,
		y - width );

	surface()->DrawFilledRect( x - height / 2,
		y + width,
		x + height / 2,
		y + width + height );

	surface()->DrawFilledRect( x - height / 2,
		y - height / 2,
		x + height / 2,
		y + height / 2 );

	// Work on Sizing later... JUST HAPPY AFTER 6  FUCKING HOURS ITS WORKING!
	surface()->DrawTexturedRect(width, height, 2, 2 );
//	surface()->DrawTexturedRect(width - crosshair_size.GetFloat(), height - crosshair_size.GetFloat(), width + crosshair_size.GetFloat(), height + crosshair_size.GetFloat());
//	surface()->DrawTexturedRect(vXHairCenter.x - iXHairHalfSize, vXHairCenter.y - iXHairHalfSize, vXHairCenter.x + iXHairHalfSize, vXHairCenter.y + iXHairHalfSize);

     SetPaintBorderEnabled(false);

	 BaseClass::PaintBackground();
}

