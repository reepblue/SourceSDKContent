//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef SDK_PLAYER_SHARED_H
#define SDK_PLAYER_SHARED_H
#ifdef _WIN32
#pragma once
#endif

// Shared header file for players
#if defined( CLIENT_DLL )
#define CSDKPlayer C_SDKPlayer	//FIXME: Lovely naming job between server and client here...
#include "template/c_sdk_player.h"
#else
#include "template/sdk_player.h"
#endif

#endif // HL2_PLAYER_SHARED_H