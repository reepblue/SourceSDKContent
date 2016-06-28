//=========== Copyright © 2013, rHetorical, All rights reserved. =============
//
// Purpose: A Player that is on top of HL2_player. Replace "rHetotical" with
// The current name. (e.g: C_rHetoricalPlayer should be C_PuntPlayer for PUNT.)
//		
//=============================================================================

#include "cbase.h"
#include "c_basehlplayer.h"

class C_rHetoricalPlayer : public C_BaseHLPlayer
{
public:
	DECLARE_CLASS( C_rHetoricalPlayer, C_BaseHLPlayer );
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	void OnDataChanged( DataUpdateType_t type );
	static C_rHetoricalPlayer* GetLocalPlayer( int nSlot = -1 );

	virtual bool ShouldRegenerateOriginFromCellBits() const
	{
		// C_BasePlayer assumes that we are networking a high-res origin value instead of using a cell
		// (and so returns false here), but this is not by default the case.
		return true; // TODO: send high-precision origin instead?
	}

	const QAngle &GetRenderAngles();

	void UpdateClientSideAnimation();

	public:
	void FireBullet( 
		Vector vecSrc, 
		const QAngle &shootAngles, 
		float vecSpread, 
		int iDamage, 
		int iBulletType,
		CBaseEntity *pevAttacker,
		bool bDoEffects,
		float x,
		float y );

private:

	QAngle m_angRender;
};

inline C_rHetoricalPlayer* To_rHetoricalPlayer( CBaseEntity *pEntity )
{
	if ( !pEntity )
		return NULL;
	Assert( dynamic_cast< C_rHetoricalPlayer* >( pEntity ) != NULL );
	return static_cast< C_rHetoricalPlayer* >( pEntity );
}
