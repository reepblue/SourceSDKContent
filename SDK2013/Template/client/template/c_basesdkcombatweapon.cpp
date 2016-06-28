//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "c_basesdkcombatweapon.h"
#include "igamemovement.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IMPLEMENT_CLIENTCLASS_DT( C_SDKMachineGun, DT_SDKMachineGun, CSDKMachineGun )
END_RECV_TABLE()

IMPLEMENT_CLIENTCLASS_DT( C_SDKSelectFireMachineGun, DT_SDKSelectFireMachineGun, CSDKSelectFireMachineGun )
END_RECV_TABLE()

IMPLEMENT_CLIENTCLASS_DT( C_BaseSDKBludgeonWeapon, DT_BaseSDKBludgeonWeapon, CBaseSDKBludgeonWeapon )
END_RECV_TABLE()