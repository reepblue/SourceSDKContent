//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef HUD_DOD_CROSSHAIR_H
#define HUD_DOD_CROSSHAIR_H
#ifdef _WIN32
#pragma once
#endif

#include "hudelement.h"
#include <vgui_controls/Panel.h>

namespace vgui
{
	class IScheme;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CHudWhiteCrosshair : public CHudElement, public vgui::Panel
{
public:
	DECLARE_CLASS_SIMPLE( CHudWhiteCrosshair, vgui::Panel );

	CHudWhiteCrosshair( const char *name );

	//virtual void OnThink();
	virtual void Paint();
	virtual void Init();
	virtual bool ShouldDraw();
	virtual void ApplySchemeSettings( vgui::IScheme *scheme );

	virtual void LevelShutdown( void );

	//stub
	void SetCrosshair( CHudTexture *texture, Color& clr ) {}
	void ResetCrosshair() {}

private:

	void	UpdateEventTime( void );

	int					m_iCrosshairTextureID;
	IVguiMatInfo		*m_pCrosshair;

	char				m_szPreviousCrosshair[256];	// name of the current crosshair
	float				m_flAccuracy;	

	CHudTexture		*m_icon_empty;
	CHudTexture		*m_icon_full;
};


// Enable/disable crosshair rendering.
extern ConVar crosshair;


#endif // HUD_DOD_CROSSHAIR_H
