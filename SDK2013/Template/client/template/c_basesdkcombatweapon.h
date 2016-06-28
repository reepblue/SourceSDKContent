//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "basesdkcombatweapon_shared.h"

#ifndef C_BASESDKCOMBATWEAPON_H
#define C_BASESDKCOMBATWEAPON_H
#ifdef _WIN32
#pragma once
#endif

class C_SDKMachineGun : public C_BaseSDKCombatWeapon
{
public:
	DECLARE_CLASS( C_SDKMachineGun, C_BaseSDKCombatWeapon );
	DECLARE_CLIENTCLASS();
};

class C_SDKSelectFireMachineGun : public C_SDKMachineGun
{
public:
	DECLARE_CLASS( C_SDKSelectFireMachineGun, C_SDKMachineGun );
	DECLARE_CLIENTCLASS();
};

class C_BaseSDKBludgeonWeapon : public C_BaseSDKCombatWeapon
{
public:
	DECLARE_CLASS( C_BaseSDKBludgeonWeapon, C_BaseSDKCombatWeapon );
	DECLARE_CLIENTCLASS();
};

#endif // C_BASESDKCOMBATWEAPON_H
