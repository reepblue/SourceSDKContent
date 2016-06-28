// ======= *********-|RECOIL|-********* =======//
// Programmer: John Stuart                                          
// ============================================//

#include "hudelement.h"
#include <vgui_controls/Panel.h>

using namespace vgui;

//-----------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------
class CEngineXhair : public CHudElement, public Panel
{
   DECLARE_CLASS_SIMPLE( CEngineXhair, Panel );

   public:

       CEngineXhair( const char *pElementName );

	   bool m_bUseThisXhair;

   protected:

       virtual void PaintBackground();
//	   void GetCurrentPos( int &x, int &y );
     //  int m_nxhair;
	   CPanelAnimationVarAliasType( int, m_nxhair, "CrosshairTexture", "vgui/crosshairs/crosshairs", "textureid" );//"vgui/hud/as_crosshair_01"
	Color				m_clrCrosshair;
	QAngle			m_curViewAngles;
	Vector			m_curViewOrigin;
};